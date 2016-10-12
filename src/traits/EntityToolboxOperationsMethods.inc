<?php

require_once ENTITY_TOOLBOX_PATH . '/src/classes/helpers/entity_toolbox/EntityToolboxHelper.inc';
require_once ENTITY_TOOLBOX_PATH . '/src/classes/helpers/entity_toolbox/ToolboxPropertiesHelper.inc';
require_once ENTITY_TOOLBOX_PATH . '/src/traits/EntityToolboxOperationsHelp.inc';

/**
 * Class EntityToolboxInfoOperationMethods
 */
trait EntityToolboxOperationMethods {
  use
    EntityToolboxCommonTraits,
    EntityToolboxDependentTraits,
    EntityToolboxPropertiesDependentTraits,
    EntityToolboxOperationsHelp {
    EntityToolboxDependentTraits::getToolboxHelper as h;
  }

  /**
   * Returns an instance of a helper.
   *
   * @param string $helperClass
   * @param array  $args
   *
   * @return HelperBase
   */
  protected function helperGetInstance($helperClass, array $args = array()) {
    $args = $this->mergeConstructArgs(array(
      'data_holder' => $this,
    ), $args);

    return new $helperClass($args);
  }

  /**
   * @return \ToolboxPropertiesHelper
   */
  protected function propsHelperGetInstance() {
    return $this->helperGetInstance('ToolboxPropertiesHelper');
  }

  /**
   * @return mixed
   */
  public function getPath(){
    return $this->path;
  }

  /**
   * @param $form
   * @param $form_state
   * @param $entity_type
   * @param $entity
   * @param $operation_path
   * @return mixed
   */
  public function entityForm($form, &$form_state, $entity_type, $entity, $operation_path, $form_op) {
    $form['form_id'] = concat($this->entityType, 'edit_form');
    properties_attach_form($entity_type, $entity, $form, $form_state);

    return $form;
  }

  /** {{@inheritdoc}} */
  public function formSubmit($form, &$form_state, $entity_type, $entity, $operation_path) {
    entity_form_submit_build_entity($entity_type, $form_state[$entity_type], $form, $form_state);
    $entity          = $form_state[$entity_type];
    $entity->changed = REQUEST_TIME;
    $entity->save();
    
    $message = $this->getOperationString('submit message', $entity_type, $entity, $operation_path);
    if (!empty($message)) {
      drupal_set_message($message);
    }

    if (empty($form_state['redirect'])) {
      $form_state['redirect'] = $this->getFormSubmitRedirect($this->entityType, $entity);
    }
  }
}