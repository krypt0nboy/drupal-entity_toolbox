<?php

require_once ENTITY_TOOLBOX_PATH . '/src/classes/operations/EntityToolboxOperationsAdd.inc';

/**
 * Class EntityToolboxOperationsAdd
 */
class EntityToolboxOperationsAddFieldable extends EntityToolboxOperationsAdd {
  /** {{@inheritdoc}} */
  public function bundleSelectionList($entity_type) {
    $items = array();
    /** @var EntityToolbox $bundle */
    foreach (entity_get_types($entity_type) as $bundle) {
      $path    = paths_join($this->basePath, array($bundle->type));
      $items[] = l(t('Add @bundle', array('@bundle' => $bundle->label())), $path);
    }
    if (empty($items)) {
      $bundleEntity      = $this->getToolboxHelper()
                                ->bundleEntity();
      $bundleEntityLabel = entity_type_label_single_lc($bundleEntity);
      $handler           = entity_operations_get_entity_operation_handler($bundleEntity, 'add');
      $message           = t('No @bundle_entity has been created. ', array(
        '@bundle_entity' => $bundleEntityLabel
      ));
      $message .= l(t('Add a @bundle_entity', array('@bundle_entity' => $bundleEntityLabel)), $handler->basePath());
      $items[]        = $message;
      $build['links'] = array(
        '#theme' => 'item_list',
        '#items' => $items,
      );
    }
    else {
      $build['links'] = array(
        '#theme' => 'item_list',
        '#items' => $items,
      );
    }

    return $build;
  }
}