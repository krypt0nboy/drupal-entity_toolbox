<?php

/**
 * @file
 *
 * This file contains default entity toolbox entity UI controller classes.
 */

/**
 * Class EntityToolboxUIController
 *
 * This class is based on \EntityDefaultUIController.
 * Either \EntityToolboxFieldableUIController or \EntityToolboxNotFieldableUIController should be implemented in code.
 */
class EntityToolboxUIController extends EntityOperationsDefaultAdminUIController {
  use
    EntityToolboxCommonTraits,
    ModuleDependentTraits,
    EntityToolboxDependentTraits,
    EntityGroupDependentTraits,
    EntityToolboxPropertiesDependentTraits;

  /**
   * @var string $wildcard
   */
  protected $wildcard;
  /**
   * @var array $formsIds
   */
  protected $formsIds = array();
  /**
   * @var array $formsSettings
   */
  protected $formsSettings = array();

  /**
   * EntityToolboxUIController constructor.
   *
   * @param string $entity_type
   *   The entity type. * Entity Toolbox provides
   * @param array  $entity_info
   *   The entity info passed by entity_get_info().
   *
   * @see entity_get_info().
   */
  public function __construct($entity_type, $entity_info) {
    parent::__construct($entity_type, $entity_info);
    $this->toolboxInfo = entity_toolbox_get_info($this->entityType);
    $this->path        = $this->toolboxInfo['root path'];
    $this->id_count    = count(explode('/', $this->path));
    $this->wildcard    = isset($this->entityInfo['admin ui']['menu wildcard']) ? $this->entityInfo['admin ui']['menu wildcard'] : "%$this->entityType";
  }

  /**
   * Returns the content for the entity add page.
   *
   * @return mixed
   */
  public function addPage() {
    $item    = menu_get_item();
    $content = system_admin_menu_block($item);
    if (count($content) == 1) {
      $item = array_shift($content);
      drupal_goto($item['href']);
    }

    return theme('entity_add_list', array(
      'type'    => $this->entityType,
      'content' => $content,
    ));
  }

}