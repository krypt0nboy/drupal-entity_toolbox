<?php

require_once ENTITY_TOOLBOX_PATH . '/src/classes/operations/EntityToolboxOperationsAdd.inc';

/**
 * Class EntityToolboxOperationsAdd
 */
class EntityToolboxOperationsAddNotFieldable extends EntityToolboxOperationsAdd {
  /** {{@inheritdoc}} */
  public function entityForm($form, &$form_state, $entity_type, $entity, $operation_path, $form_op) {
    $form         = parent::entityForm($form, $form_state, $entity_type, $entity, $operation_path, $form_op);
    $form['type'] = array(
      '#type'          => 'machine_name',
      '#default_value' => isset($entity_type->type) ? $entity_type->type : '',
      '#maxlength'     => 32,
      '#machine_name'  => array(
        'exists' => preg_replace('/_type/', '', $this->entityType) . '_get_types',
        'source' => array(
          $this->getPropsHelper()
               ->propGetValueParam('type', 'source')
        ),
      ),
    );

    return $form;
  }
}