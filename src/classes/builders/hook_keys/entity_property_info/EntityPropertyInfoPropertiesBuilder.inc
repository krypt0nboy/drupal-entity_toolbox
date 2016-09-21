<?php

/**
 * Class EntityPropertyInfoPropertiesBuilder
 */
class EntityPropertyInfoPropertiesBuilder extends EntityPropertyInfoKeyBuilder {

  /** {{@inheritdoc}} */
  public function __construct(array $args = array()) {
    $args = $this->mergeConstructArgs(
      array(
        'self_key' => 'properties',
      ),
      $args
    );
    parent::__construct($args);
  }

  /** {{@inheritdoc}} */
  protected function constructSet($args) {
    parent::constructSet($args);
    $this->__set('properties', $this->toolboxInfo['properties']);
  }

  /** {{@inheritdoc}} */
  protected function genDynamicStructure() {
    $structure          = array();
    $keyBuildersClasses = array();
    $keyBuildersArgs    = array();
    $helper             = $this->getPropsHelper();
    foreach ($this->properties as $property => $info) {
      $isRef                = $helper->propIsReference($property);
      $isMulti              = $helper->propIsMulti($property);
      $structure[$property] = array(
        'type'   => 'array',
        'method' => 'buildProperty',
      );
      $keyBuilderClass      = 'EntityPropertyInfoPropertyBuilder';
      if ($isRef) {
        $keyBuilderClass = $isMulti ? 'EntityPropertyInfoReferenceMultiplePropertyBuilder' : 'EntityPropertyInfoReferencePropertyBuilder';
      }
      $keyBuildersArgs[$property]    = array(
        'property_type' => $this->toolboxInfo['properties'][$property]['type'],
        'property_name' => $property,
      );
      $keyBuildersClasses[$property] = $keyBuilderClass;
      if ($isRef) {
        $numProp                      = $info['self_numeric_property'];
        $structure[$numProp]          = array(
          'type'   => 'array',
          'method' => 'buildProperty',
        );
        $numPropBuilderClass          = $isMulti ? 'EntityPropertyInfoReferenceMultipleNumericPropertyBuilder' : 'EntityPropertyInfoReferenceNumericPropertyBuilder';
        $keyBuildersClasses[$numProp] = $numPropBuilderClass;
        $keyBuildersArgs[$numProp]    = array(
          'property_name'           => $numProp,
          'reference_property_name' => $property,
          'reference'               => $info['reference'],
          'reference_info'          => $this->entityGetCachedInfo($info['reference']),
        );
      }
    }
    $this->__set('key_builders_classes', $keyBuildersClasses);
    $this->__set('key_builders_args', $keyBuildersArgs);

    return $structure;
  }

  /** {{@inheritdoc}} */
  protected function alterStructure() {

  }

  /**
   * Extracts and set the reference properties name.
   *
   * @param bool $autoSet
   *   A boolean indicating if the reference properties should be set when extracted.
   *
   * @return array
   */
  protected function extractReferenceProperties($autoSet = FALSE) {
    $extract = array();
    foreach ($this->properties as $property => $info) {
      if (!empty($info['reference'])) {
        $extract[] = $property;
      }
    }
    if ($autoSet) {
      $this->setReferenceProperties($extract);
    }

    return $this->referenceProperties;
  }

  /**
   * @param array $args
   * @return \BuilderBase
   */
  public function addProperty(array $args = array()) {
    $structure                         = array();
    $structure[$args['property_name']] = array(
      'type'   => 'array',
      'method' => 'buildProperty',
    );
    $keyBuilderClass                   = $args['key_builder_class'];
    $keyBuilderArgs                    = array(
      'property_type'              => $args['property_type'],
      'property_name'              => $args['property_name'],
      'reference'                  => $args['reference'],
      'reference_info'             => $args['reference_info'],
      'reference_property_builder' => $args['reference_property_builder'],
    );
    $this->structure += $structure;
    $keyBuilder = $this->keyBuilderGetInstance($keyBuilderClass, $keyBuilderArgs);
    $this->setKeyBuilder($args['property_name'], $keyBuilder);

    return $this->getKeyBuilder($args['property_name']);
  }

  /** {{@inheritdoc}} */
  public function build() {
    $this->setPhase(self::PHZ__BUILD);
    $this->preBuild();
    foreach (array_keys($this->structure) as $key) {
      $this->setKeyBuild($key, $this->buildProperty($key));
    }
    $this->validateBuild();
    $this->postBuild();

    return $this->deliver();
  }

  /**
   * @param $property
   * @return array
   */
  protected function buildProperty($property) {
    return $this->getKeyBuilder($property)->build();
  }
}