<?php

/**
 * Class EntityOperationInfoKeyBuilder
 */
class EntityOperationInfoKeyBuilder extends EntityToolboxDependentHookKeyBuilder {
  /** {{@inheritdoc}} */
  public function __construct(array $args = array()) {
    $args = $this->mergeConstructArgs(
      array(
        'hook' => 'entity_operation_info',
      ),
      $args
    );
    parent::__construct($args);
  }
}