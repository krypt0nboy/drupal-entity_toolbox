<?php

/**
 * Class EntityToolboxDependentTraits
 */
trait EntityToolboxDependentTraits {
  /**
   * @var array $toolboxInfo
   *   Toolbox info retrieved and processed by entity toolbox.
   *
   * @see hook_entity_toolbox_entity_info().
   * @see entity_toolbox_get_info().
   */
  protected $toolboxInfo = array();
  /**
   * @var EntityToolboxHelper $toolboxHelper
   *   A helper for toolboxInfo.
   */
  protected $toolboxHelper;

  /**
   * @return array
   */
  public function getToolboxInfo() {
    return $this->toolboxInfo;
  }

  /**
   * @param array $toolboxInfo
   */
  public function setToolboxInfo($toolboxInfo) {
    $this->toolboxInfo = $toolboxInfo;
  }

  /**
   * @return EntityToolboxHelper
   */
  public function getToolboxHelper() {
    return $this->toolboxHelper;
  }

  /**
   * @param EntityToolboxHelper $toolboxHelper
   */
  public function setToolboxHelper($toolboxHelper) {
    $this->toolboxHelper = $toolboxHelper;
  }

  /**
   * Auto sets toolboxInfo.
   */
  public function autoSetToolboxInfo() {
    $this->setToolboxInfo(entity_toolbox_get_info($this->entityType));
  }
}