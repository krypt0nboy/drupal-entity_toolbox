<?php

/**
 * Class MenuRecursiveModel
 */
class MenuRecursiveModel extends HookRecursiveModel {
  /** {{@inheritdoc}} */
  public function __construct(array $args = array()) {
    $args = $this->mergeConstructArgs(
      array(
        'hook' => 'menu',
      ), $args
    );
    parent::__construct($args);
  }
}