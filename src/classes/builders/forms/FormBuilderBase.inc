<?php

/**
 * Class FormBuilderBase
 */
class FormBuilderBase extends BuilderBase {
  /** {{@inheritdoc}} */
  public function __construct(array $args = array()) {
    $args = $this->mergeConstructArgs(array(
      'model_class' => 'FormModel'
    ), $args);
    parent::__construct($args);
  }
}