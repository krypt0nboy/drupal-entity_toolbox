<?php

/**
 * Class PropertyValueBuilder
 */
class EntityToolboxInfoPropertyValueBuilder extends EntityToolboxInfoPropertyDependentKeyBuilder {
  /** {{@inheritdoc}} */
  public function __construct(array $args = array()) {
    $args = $this->mergeConstructArgs(
      array(
        'self_key'    => 'classes',
        'model_class' => 'EntityToolboxInfoPropertyValueModel'
      ),
      $args
    );
    parent::__construct($args);
  }

  /**
   * Builds the value "filter".
   *
   * @return array
   */
  protected function buildFilter() {
  }

  /**
   * Builds the value "forbid".
   *
   * @return bool
   */
  protected function buildForbid() {
  }

  protected function buildSource() {
    if (array_key_exists('property_default_value_source', $this->getTemplateVars())) {
      return $this->getTemplateVars('property_default_value_source');
    }
  }
}