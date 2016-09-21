<?php

/**
 * Class MenuBuilder
 */
class MenuBuilder extends HookBuilder {
  use ModuleDependentTraits;

  /** {{@inheritdoc}} */
  public function __construct(array $args = array()) {
    $args = $this->mergeConstructArgs(array(
      'hook' => 'menu',
    ), $args);
    parent::__construct($args);
  }

  /**
   * Builds the module's entity group menu items.
   *
   * @param string $entityGroup
   *   The entity group.
   *
   * @return array
   */
  public function buildEntityGroupMenuItems($entityGroup) {
  }

  /** {{@inheritdoc}} */
  protected function genDynamicStructure() {
    $structure          = array();
    $keyBuildersClasses = array();
    $keyBuildersArgs    = array();

    $this->__set('key_builders_classes', $keyBuildersClasses);
    $this->__set('key_builders_args', $keyBuildersArgs);

    return $structure;
  }

  /**
   * @return array
   */
  protected function extractEntityGroupsMenuItemsArgs() {
    $args = array();
    foreach ($this->moduleInfo->entity_groups as $group) {
      $args['structure']         = array(
        'type'   => 'array',
        'method' => 'buildEntityGroupMenuItems'
      );
      $args['key_builder_class'] = 'EntityGroupMenuItemsBuilder';
    }

    return $args;
  }

}