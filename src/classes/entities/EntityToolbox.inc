<?php

/**
 * @file
 *
 * This file contains entity_toolbox entity classes.
 */

/**
 * Class EntityToolbox
 */
class EntityToolbox extends Entity {
  use
    EntityToolboxCommonTraits,
    EntityToolboxDependentTraits,
    EntityParametersDependentTraits,
    EntityToolboxPropertiesDependentTraits;

  /**
   * @var string $entityGroup
   *   The entity group.
   *
   * @see hook_group_attach_info().
   */
  protected $entityGroup;

  /**
   * Constructor.
   *
   * @param array $values
   *   Values.
   * @param       $entity_type
   *   The entity type.
   *
   * @throws \Exception
   */
  public function __construct($values = array(), $entity_type = NULL) {
    parent::__construct($values, $entity_type);
    $this->autoSetToolboxInfo();
  }

  /**
   * {@inheritdoc}
   */
  public function setUp() {
    parent::setUp();
    $helper = $this->getToolboxHelper();
    $helper->dataToArgs($args = array());
    $this->constructSet($args);
  }

  /**
   * Init phase where construct args are set.
   *
   * @param array $args
   */
  protected function constructSet($args) {
    $this->setConstructArgs($args);
    $this->bulkSet($args);
  }

  /**
   * @return string
   */
  public function getEntityGroup() {
    return $this->entityGroup;
  }

  /**
   * @param string $entityGroup
   */
  public function setEntityGroup($entityGroup) {
    $this->entityGroup = $entityGroup;
  }

  /**
   * Return the entity URI.
   *
   * @return array
   *   Uri.
   */
  protected function defaultUri() {
    return array('path' => "$this->entityType/" . $this->{$this->idKey});
  }
}