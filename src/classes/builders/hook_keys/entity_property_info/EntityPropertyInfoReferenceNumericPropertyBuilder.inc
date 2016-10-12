<?php

/**
 * Class EntityPropertyInfoReferenceNumericPropertyBuilder
 */
class EntityPropertyInfoReferenceNumericPropertyBuilder extends EntityPropertyInfoPropertyBuilder {
  use ReferenceDependentTraits, ReferenceDependentBuilderTraits;
  /**
   * @var EntityPropertyInfoReferencePropertyBuilder
   */
  protected $referencePropertyBuilder;
  /**
   * @var string $referencePropertyName
   */
  protected $referencePropertyName;

  /** {{@inheritdoc}} */
  public function __construct(array $args = array()) {
    $args = $this->mergeConstructArgs(
      array(
        'property_type' => 'integer',
        'model_class'   => 'EntityPropertyInfoReferenceNumericPropertyModel',
      ),
      $args
    );
    parent::__construct($args);
  }

  /** {{@inheritdoc}} */
  protected function autoSet() {
    parent::autoSet();
    $this->autoSetReferencePropertyBuilder();
  }

  /**
   * Auto sets propertyInfo.
   */
  protected function autoSetPropertyInfo() {
    $propInfo = $this->properties[$this->referencePropertyName];
    $this->__set('property_info', $propInfo);
  }

  /**
   * Auto sets referencePropertyBuilder.
   */
  protected function autoSetReferencePropertyBuilder() {
    $parentBuilder = $this->getParentBuilder();
    $hlp           = $parentBuilder->hlpr();
    if ($hlp->keyHasBuilder($this->referencePropertyName)) {
      /** @var EntityPropertyInfoReferencePropertyBuilder $builder */
      $builder = $hlp->keyGetBuilder($this->referencePropertyName);
      $this->__set('reference_property_builder', $builder);
      if (!$builder->__isset('numeric_property_builder')) {
        $builder->setNumericPropertyBuilder($this);
      }
    }
  }

  /** {{@inheritdoc}} */
  protected function initTemplateVars(array $vars = array()) {
    $vars += $this->extractReferenceInfoTemplateVars();
    if(isset($this->referencePropertyBuilder)){
      $vars += $this->getReferencePropertyBuilder()
                    ->extractToolboxInfoPropertyTemplateVars();
    }

    return EntityDependentHookKeyBuilder::initTemplateVars($vars);
  }

  /**
   * @return EntityPropertyInfoReferencePropertyBuilder
   */
  public function getReferencePropertyBuilder() {
    return $this->referencePropertyBuilder;
  }

  /**
   * @param mixed $referencePropertyBuilder
   */
  public function setReferencePropertyBuilder($referencePropertyBuilder) {
    $this->referencePropertyBuilder = $referencePropertyBuilder;
  }

  /**
   * @return string
   */
  public function getReferencePropertyName() {
    return $this->referencePropertyName;
  }

  /**
   * @param string $referencePropertyName
   */
  public function setReferencePropertyName($referencePropertyName) {
    $this->referencePropertyName = $referencePropertyName;
  }

  /** {{@inheritdoc}} */
  protected function buildType() {
    return 'integer';
  }

  /** {{@inheritdoc}} */
  protected function buildSchemaField() {
    $name               = $this->referencePropertyName;
    $helper             = $this->getPropsHelper();
    $isMulti            = $helper->propIsMulti($name);
    $isRef              = $helper->propIsReference($name);
    $hasBaseSchemaField = $helper->propHasSchemaFieldName($name, 'base');
    if (!$isMulti && !$isRef && $hasBaseSchemaField) {
      return $helper->propGetSchemaFieldName($name, 'base');
    }
  }

  /**
   * @return array
   */
  protected function buildClear() {
    return array($this->referencePropertyBuilder->getPropertyName());
  }
}