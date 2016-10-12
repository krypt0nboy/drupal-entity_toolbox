<?php

/**
 * Class OperationsBuilderNotFieldable
 */
class EntityToolboxInfoOperationsBuilderNotFieldable extends EntityToolboxInfoOperationsBuilder {
  /** {{@inheritdoc}} */
  public function __construct(array $args = array()) {
    $args = $this->mergeConstructArgs(array(
      'required_operations' => array(
        'add'  => array('handler' => 'EntityToolboxOperationsAddNotFieldable'),
        'edit' => array('handler' => 'EntityToolboxOperationsEditNotFieldable'),
        'view' => array('handler' => 'EntityToolboxOperationsViewNotFieldable'),
      )
    ), $args);
    parent::__construct($args);
  }
}