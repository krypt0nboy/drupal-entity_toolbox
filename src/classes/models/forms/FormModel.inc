<?php

/**
 * Class FormModel
 */
class FormModel extends ModelBase {
  /** {{@inheritdoc}} */
  public function __construct(array $args = array()) {
    $args = $this->mergeConstructArgs(array(
      'structure' => array(
        'elements' => array(
          'type'     => 'array',
          'method'   => '',
          'default'  => '',
          'model'    => '',
          'unset if' => '',
        ),
        ''         => array(
          'type'     => '',
          'method'   => '',
          'default'  => '',
          'model'    => '',
          'unset if' => '',
        ),
      )
    ), $args);
    parent::__construct($args);
  }
}

