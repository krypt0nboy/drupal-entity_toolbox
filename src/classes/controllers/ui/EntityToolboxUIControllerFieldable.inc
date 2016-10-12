<?php

/**
 * Class EntityToolboxUIControllerFieldable
 *
 * The default UI Controller for fieldable entities.
 */
class EntityToolboxUIControllerFieldable extends EntityToolboxUIController {
  /** {{@inheritdoc}} */
  public function hook_menu() {
    $items                                                  = parent::hook_menu();
    $items[concat($this->entityType, $this->wildcard, '/')] = array(
      'title callback'   => $this->entityType . '_page_title',
      'title arguments'  => array(1),
      'page callback'    => $this->entityType . '_page_view',
      'page arguments'   => array(1),
      'access callback'  => entity_type_callbacks($this->entityType, 'access'),
      'access arguments' => array('view', 1),
      'type'             => MENU_CALLBACK,
    );

    return $items;
  }
}