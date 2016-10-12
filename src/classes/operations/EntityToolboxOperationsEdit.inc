<?php

require_once ENTITY_TOOLBOX_PATH . '/src/traits/EntityToolboxOperationsMethods.inc';
require_once ENTITY_TOOLBOX_PATH . '/src/traits/EntityToolboxOperationsProperties.inc';

/**
 * Class EntityToolboxOperationsEdit
 */
class EntityToolboxOperationsEdit extends EntityOperationsOperationEditGeneric {
  use
    EntityToolboxOperationMethods,
    EntityToolboxOperationsProperties{
    EntityToolboxOperationMethods::entityForm as genericForm;
    EntityToolboxOperationMethods::formSubmit as genericFormSubmit;
  }

  /**
   * EntityToolboxOperationsAdd constructor.
   * @param $entity_type
   * @param $operation_path
   */
  public function __construct($entity_type, $operation_path) {
    parent::__construct($entity_type, $operation_path);
    $this->setUp();
  }

  /**
   * Set up method.
   */
  public function setUp() {
    $this->autoSetToolboxInfo();
    $this->autoSetProperties();
    $this->setToolboxHelper(new EntityToolboxHelper(array('data_holder' => $this)));
    $this->setPropsHelper($this->propsHelperGetInstance());

    $h              = $this->h();
    $this->basePath = paths_join($h->rootPath(), array($this->path));
  }

  /** {{@inheritdoc}} */
  public function userAccess($entity_type, $entity, $params = array(), $account = NULL) {
    //@todo actually handle access
    return TRUE;
  }

  /** {{@inheritdoc}} */
  public function entityForm($form, &$form_state, $entity_type, $entity, $operation_path, $form_op) {
    $form = parent::entityForm($form, $form_state, $entity_type, $entity, $operation_path, $form_op);
    $form += $this->genericForm($form, $form_state, $entity_type, $entity, $operation_path, $form_op);

    return $form;
  }

  /** {{@inheritdoc}} */
  public function formSubmit($form, &$form_state, $entity_type, $entity, $operation_path) {
    $this->genericFormSubmit($form, $form_state, $entity_type, $entity, $operation_path);
  }
}