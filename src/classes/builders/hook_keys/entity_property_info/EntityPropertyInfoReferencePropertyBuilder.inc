<?php

/**
 * Class EntityPropertyInfoReferencePropertyBuilder
 */
class EntityPropertyInfoReferencePropertyBuilder extends EntityPropertyInfoPropertyBuilder {
  use ReferenceDependentTraits, ReferenceDependentBuilderTraits;

  /**
   * @var EntityPropertyInfoReferenceNumericPropertyBuilder $numericPropertyBuilder
   */
  protected $numericPropertyBuilder;

  /** {{@inheritdoc}} */
  public function __construct(array $args = array()) {
    parent::__construct($args);
  }

  /** {{@inheritdoc}} */
  protected function autoSet() {
    parent::autoSet();
    $this->autoSetNumericPropertyBuilder();
  }

  /** {{@inheritdoc}} */
  protected function initTemplateVars(array $vars = array()) {
    $vars += $this->extractReferenceInfoTemplateVars();
    parent::initTemplateVars($vars);
  }

  /**
   * @return mixed
   */
  public function getNumericPropertyBuilder() {
    return $this->numericPropertyBuilder;
  }

  /**
   * @param EntityPropertyInfoReferenceNumericPropertyBuilder $numericPropertyBuilder
   */
  public function setNumericPropertyBuilder($numericPropertyBuilder) {
    $this->numericPropertyBuilder = $numericPropertyBuilder;
  }

  /**
   * Auto sets numericPropertyBuilder.
   */
  protected function autoSetNumericPropertyBuilder() {
    $numPropertyName = $this->propertyInfo['self_numeric_property'];
    $parentBuilder   = $this->getParentBuilder();
    $hlp             = $parentBuilder->hlpr();
    if ($hlp->keyHasBuilder($numPropertyName)) {
      /** @var EntityToolboxInfoReferenceNumericPropertyBuilder $numPropBuilder */
      $numPropBuilder = $hlp->keyGetBuilder($numPropertyName);
      $this->__set('numeric_property_builder', $numPropBuilder);
      if (!$numPropBuilder->__isset('reference_property_builder')) {
        $numPropBuilder->setReferencePropertyBuilder($this);
      }
    }
  }

  /**
   * @return mixed
   */
  public function genNumericPropertyName() {
    return $this->propertyInfo['self_numeric_property'];
  }

  /** {{@inheritdoc}} */
  protected function buildSchemaField() {
  }

  /**
   * @return array
   */
  protected function buildClear() {
    return array($this->numericPropertyBuilder->getPropertyName());
  }
}