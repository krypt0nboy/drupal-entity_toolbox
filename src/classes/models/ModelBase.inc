<?php

/**
 * Class ModelBase
 */
class ModelBase {
  use EntityToolboxCommonTraits;
  /**
   * @access protected
   * @var array $structure
   *   The model data structure.
   */
  protected $structure = array();

  /**
   * ModelBase constructor.
   *
   * @param array $args
   */
  public function __construct(array $args = array()) {
    $this->constructSet($args);
  }

  /**
   * @param $args
   */
  protected function constructSet($args) {
    $this->setConstructArgs($args);
    foreach ($args as $argName => $arg) {
      $this->__set($argName, $arg);
    }
  }

  /**
   * Setter for "structure".
   *
   * @param array $structure
   *   The structure to set.
   */
  protected function setStructure($structure) {
    $this->structure = $structure;
  }

  /**
   * Getter for "structure".
   *
   * @return array
   */
  public function getStructure() {
    return $this->structure;
  }

}