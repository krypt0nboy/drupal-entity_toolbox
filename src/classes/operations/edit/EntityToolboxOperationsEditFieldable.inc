<?php

require_once ENTITY_TOOLBOX_PATH . '/src/classes/operations/EntityToolboxOperationsEdit.inc';

/**
 * Class EntityToolboxOperationsAdd
 */
class EntityToolboxOperationsEditFieldable extends EntityToolboxOperationsEdit {
  /** {{@inheritdoc}} */
  public function bundleSelectionList($entity_type) {
    $items = array();
    /** @var EntityToolbox $bundle */
    foreach (entity_get_types($entity_type) as $bundle) {
      $path    = paths_join($this->basePath, array($bundle->type));
      $items[] = l(t('Add @bundle', array('@bundle' => $bundle->label())), $path);
    }
    $build['links'] = array(
      '#theme' => 'item_list',
      '#items' => $items,
    );

    return $build;
  }
}