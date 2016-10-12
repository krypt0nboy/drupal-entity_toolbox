<?php

/**
 * Class EntityToolboxInfoPropertySchemaFieldBuilder
 */
class EntityToolboxInfoPropertySchemasFieldBuilder extends EntityToolboxInfoPropertiesSchemaDependentKeyBuilder {
  /** {{@inheritdoc}} */
  public function __construct(array $args = array()) {
    $args = $this->mergeConstructArgs(
      array(
        'model_class' => 'EntityToolboxInfoPropertySchemasFieldModel',
      ),
      $args
    );
    parent::__construct($args);
  }

  /** {{@inheritdoc}} */
  protected function modelGetInstance($modelClass, array $args = array()) {
    $args = $this->mergeConstructArgs(array(
      'schema_type' => $this->constructArgs['schema_type']
    ), $args);

    return parent::modelGetInstance($modelClass, $args);
  }

  /**
   * @param mixed $build
   * @return mixed
   */
  protected function vedKeySize($build) {
    if (($this->isValueTemplate($build)) || (!is_string($build)) || (empty($build))) {
      return 0;
    }
    else {
      return $build;
    }
  }

  /**
   * @param mixed $build
   * @return mixed
   */
  protected function vedKeyScale($build) {
    if (($this->isValueTemplate($build)) || (is_string($build)) || (empty($build))) {
      return 0;
    }
    else {
      return $build;
    }
  }

  /**
   * @param mixed $build
   * @return mixed
   */
  protected function vedKeyPrecision($build) {
    if (($this->isValueTemplate($build)) || (is_string($build)) || (empty($build))) {
      return 0;
    }
    else {
      return $build;
    }
  }

  /**
   * @param mixed $build
   * @return mixed
   */
  protected function vedKeyLength($build) {
    return !is_integer($build) ? 0 : $build;
  }
}