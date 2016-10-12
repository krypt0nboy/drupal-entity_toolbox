<?php

/**
 * Class EntityInfoAlterBuilder
 */
class EntityInfoAlterBuilder extends EntityToolboxDependentHookBuilder {
  /**
   * @access protected
   * @var array $data2Alter
   *   The data to alter.
   */
  protected $data2Alter = array();

  /**
   * @return array
   */
  public function getData2Alter() {
    return $this->data2Alter;
  }

  /**
   * @param array $data2Alter
   */
  public function setData2Alter($data2Alter) {
    $this->data2Alter = $data2Alter;
  }

  /**
   * Returns the precise data to alter.
   *
   * @return mixed
   */
  protected function &data2AlterSelective() {
    return $this->data2Alter[$this->entityType];
  }

  /**
   * Alter method.
   *
   * @param array $info
   */
  public function alter(&$info) {
    $this->setData2Alter($info);
  }

  /**
   * @param $key
   * @param $value
   */
  protected function alterData($key, $value) {

  }
}