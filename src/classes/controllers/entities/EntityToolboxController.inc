<?php

require_once ENTITY_TOOLBOX_PATH . '/src/classes/helpers/entity_toolbox/EntityToolboxHelper.inc';
require_once ENTITY_TOOLBOX_PATH . '/src/classes/helpers/entity_toolbox/ToolboxPropertiesHelper.inc';
require_once ENTITY_TOOLBOX_PATH . '/src/traits/EntityParametersDependentTraits.inc';

/**
 * The Controller for entities managed by entity toolbox
 *
 * Class EntityToolboxEntityController
 */
class EntityToolboxController extends EntityAPIController {

  use
    EntityToolboxCommonTraits,
    ModuleDependentTraits,
    EntityToolboxDependentTraits,
    EntityParametersDependentTraits,
    EntityGroupDependentTraits,
    EntityToolboxPropertiesDependentTraits;

  /**
   * Constructor.
   *
   * @param string $entity_type
   *   Entity type.
   */
  public function __construct($entity_type) {
    parent::__construct($entity_type);
    $this->autoSetToolboxInfo();
    $this->setUp();
  }

  /**
   * Set up method.
   *
   * @param array $args
   *   Arguments to set.
   */
  protected function setUp(array $args = array()) {
    $this->autoSetHelpers();
    $helper = $this->getToolboxHelper();
    $helper->dataToArgs($args);
    $this->constructSet($args);
    $this->autoSet();
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
   * Auto sets properties.
   */
  protected function autoSet() {
  }

  /**
   * Auto sets required helpers.
   */
  protected function autoSetHelpers() {
    $this->setToolboxHelper($this->toolboxHelperGetInstance());
    $this->setPropsHelper($this->propsHelperGetInstance());
  }

  /**
   * Create an entity.
   *
   * @param array $values
   *   The values of the entity.
   *
   * @return object
   *   An entity object with all default fields initialized.
   */
  public function create(array $values = array()) {
    $values += $this->propertiesDefaultValues();
    $entity = parent::create($values);

    return $entity;
  }

  /**
   * @return string[]
   */
  public function getReferenceProperties() {
    return $this->referenceProperties;
  }

  /**
   * @param string[] $referenceProperties
   */
  public function setReferenceProperties($referenceProperties) {
    $this->referenceProperties = $referenceProperties;
  }

  /**
   * Returns the parameters of a relation table between two entity types.
   *
   * @param \EntityToolbox $entity
   *   The entity.
   * @param string         $name
   *   The property name.
   *
   * @return array
   */
  public function relationExtractInfo($entity, $name) {
    $helper = $this->getPropsHelper();

    return array(
      'self_id_key' => $this->idKey,
      'self_id'     => $entity->{$this->idKey},
      'rel_table'   => $helper->propGetRelationTable($name),
      'ref_rel_key' => $helper->propGetSchemaField($name, 'relation'),
    );
  }

  /**
   * Extracts the parameters for a relation table query.
   *
   * @param EntityToolbox $entity
   *   The entity.
   * @param string        $name
   *   The reference property.
   *
   * @return array
   */
  public function relationQueryExtractFields($entity, $name) {
    $helper   = $this->getPropsHelper();
    $fields   = array();
    $required = array(
      $helper->propsGetIdProperty(),
      $name
    );
    foreach ($required as $prop) {
      $fieldName = $helper->propGetSchemaFieldName($prop, 'relation');
      $fields[]  = array($fieldName => '');
    }

    return $fields;
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
   * @return \EntityToolboxHelper
   */
  protected function toolboxHelperGetInstance() {
    return $this->helperGetInstance('EntityToolboxHelper');
  }

  /**
   * Inserts values for a multi value reference property.
   *
   * @param \EntityToolbox $entity
   *   The entity.
   * @param string         $name
   *   The property name.
   * @param int[]          $add_ids
   *   A list of numeric ids to insert as values.
   *
   * @return array
   * @throws \Exception
   */
  public function insertMultiReferenceValues($entity, $name, $add_ids = array()) {
    $helper        = $this->getPropsHelper();
    $table         = $helper->propGetRelationTable($name);
    $entityIdField = $this->idKey;
    $entityId      = $entity->{$this->idKey};
    $refField      = $helper->propGetSchemaField($name, 'relation');
    $inserted      = array();
    foreach ($add_ids as $add_id) {
      $inserted[] = db_insert($table)
        ->fields(
          array(
            $entityIdField => $entityId,
            $refField      => $add_id
          )
        )
        ->execute();
    }

    return $inserted;
  }

  /**
   * Deletes values of a multi value reference property.
   *
   * @param \EntityToolbox $entity
   *   The entity.
   * @param string         $name
   *   The property name.
   * @param int[]          $del_ids
   *   The references to delete.
   */
  public function deleteMultiReferenceValues($entity, $name, $del_ids = array()) {
    list($self_id_key, $self_id, $rel_table, $ref_rel_key) = $this->relationExtractInfo($entity, $name);
    db_delete($rel_table)
      ->condition($self_id_key, $self_id)
      ->condition($ref_rel_key, $del_ids, 'IN')
      ->execute();
  }

  /**
   * Returns a property default value.
   *
   * @param string $name
   *   The property name.
   *
   * @return mixed
   */
  public function propertyDefaultValue($name) {
    $helper = $this->getPropsHelper();

    return $helper->propGetDefaultValue($name);
  }

  /**
   * Returns a property value of an existing entity.
   *
   * @param EntityToolbox $entity
   *   The entity.
   * @param string        $name
   *   The property name.
   *
   * @return mixed
   */
  public function propertyValue($entity, $name) {

  }

  /**
   * Returns properties default values.
   *
   * @return array
   */
  public function propertiesDefaultValues() {
    $values = array();
    $helper = $this->getPropsHelper();
    foreach ($this->properties as $name => $info) {
      $key = $helper->propHasKey($name);
      if ($key && $helper->propHasDefaultValue($name)) {
        $key          = $this->keyGet($name);
        $values[$key] = $this->propertyDefaultValue($name);
      }
    }

    return $values;
  }

  /**
   * @param $property
   * @return mixed
   */
  public function keyGet($property) {
    $helper     = $this->getPropsHelper();
    $entityKeys = $this->entityInfo['entity keys'];
    $key        = $helper->propGetKey($property);

    return $entityKeys[$key];
  }

  /**
   * Returns a boolean indicating if a property is a list of values.
   *
   * @param string $name
   *   The property name.
   *
   * @return bool
   */
  public function isPropertyMultiValue($name) {
    $helper = $this->getPropsHelper();

    return $helper->propIsMulti($name);
  }

  /**
   * Validates an entity property.    $property    = $this->toolboxInfo['properties'][$name];
   *
   * @param \EntityToolbox $entity
   *   The entity.
   * @param string         $name
   *   The property name.
   * @param array          $form
   *   The entity form, passed by reference
   * @param array          $form_state
   *   The form state, passed by reference.
   */
  public function propertyValidate(&$entity, $name, &$form, &$form_state) {
    //@todo gather the property validation callbacks
    _property_validate($this->entityType, $entity, $name, $form, $form_state);
  }

  /**
   * Submits an entity property.
   *
   * @param \EntityToolbox $entity
   *   The entity.
   * @param string         $name
   *   The property name.
   * @param array          $form
   *   The entity form, passed by reference
   * @param array          $form_state
   *   The form state, passed by reference.
   */
  public function propertySubmit(&$entity, $name, &$form, &$form_state) {
    _property_submit($this->entityType, $entity, $name, $form, $form_state);
  }

  /**
   * Returns a list of ids from a list of references.
   *
   * @param string $name
   *   The property name for which to return the list.
   * @param array  $references
   *   The references list.
   *
   * @return integer[]
   */
  public function propertyReferences2ReferenceIds($name, array $references = array()) {
    $helper        = $this->getPropsHelper();
    $referenceType = $helper->propGetReference($name);

    return references2references_ids($references, $referenceType);
  }

  /**
   * Sets an entity multi value property.
   *
   * @param \EntityToolbox $entity
   *   The entity for which to set the value.
   * @param string         $name
   *   The property name.
   * @param array          $values
   *   The values to set.
   * @param null|string    $langcode
   *   The language code.
   * @param array          $options
   *   The set options.
   */
  public function setMultiValueProperty($entity, $name, $values, $langcode = NULL, $options = array()) {
    //@todo handle serialized multi value.
    //@todo handle non referential multi value.
    //@todo add hook_property_pre_set
    //@todo add hook_property_post_set
    $helper = $this->getPropsHelper();
    if ($helper->propIsReference($name)) {
      $this->setMultiValueReferenceProperty($entity, $name, $values, $langcode = NULL, $options = array());
    }
    else {
      $values  = is_array($values) ? $values : array();
      $cur_ids = $this->getMultiValueProperty($entity, $name, NULL, $langcode, $options);
      $del_ids = array_diff($cur_ids, $values);
      $add_ids = array_diff($values, $cur_ids);
      //@todo add hook_property_pre_set
      if (!empty($add_ids)) {
        $this->insertMultiReferenceValues($entity, $name, $add_ids);
      }
      if (!empty($del_ids)) {
        $this->deleteMultiReferenceValues($entity, $name, $del_ids);
      }
    }
  }

  /**
   * Sets an entity multi value property.
   *
   * @param \EntityToolbox $entity
   *   The entity for which to set the value.
   * @param string         $name
   *   The property name.
   * @param array          $values
   *   The values to set.
   * @param null|string    $langcode
   *   The language code.
   * @param array          $options
   *   The set options.
   */
  public function setMultiValueReferenceProperty($entity, $name, $values = array(), $langcode = NULL, $options = array()) {
    $values = is_array($values) ? $values : array();
    if (!empty($values)) {
      $values = $this->propertyReferences2ReferenceIds($name, $values);
    }
    $cur_ids = $this->getMultiValueProperty($entity, $name, NULL, $langcode, $options);
    $del_ids = array_diff($cur_ids, $values);
    $add_ids = array_diff($values, $cur_ids);
    //@todo add hook_property_pre_set
    if (!empty($add_ids)) {
      $this->insertMultiReferenceValues($entity, $name, $add_ids);
    }
    if (!empty($del_ids)) {
      $this->deleteMultiReferenceValues($entity, $name, $del_ids);
    }
    //@todo add hook_property_post_set
  }

  /**
   * Adds a/several value(s) to an entity multi value property.
   *
   * @param \EntityToolbox $entity
   *   The entity for which to set the value.
   * @param string         $name
   *   The property name.
   * @param array          $values
   *   The values to set.
   * @param null|string    $langcode
   *   The language code.
   * @param array          $options
   *   The set options.
   */
  public function addValueMultiValueReferenceProperty($entity, $name, $values = array(), $langcode = NULL, $options = array()) {
    $values = is_array($values) ? $values : array();
    if (!empty($values)) {
      $values = $this->propertyReferences2ReferenceIds($name, $values);
    }
    $cur_ids = $this->getMultiValueProperty($entity, $name, NULL, $langcode, $options);
    $add_ids = array_diff($values, $cur_ids);

    if (!empty($add_ids)) {
      $this->insertMultiReferenceValues($entity, $name, $add_ids);
    }
  }

  /**
   * REmoves a/several value(s) to an entity multi value property.
   *
   * @param \EntityToolbox $entity
   *   The entity for which to set the value.
   * @param string         $name
   *   The property name.
   * @param array          $values
   *   The values to set.
   * @param null|string    $langcode
   *   The language code.
   * @param array          $options
   *   The set options.
   */
  public function deleteValueMultiValueReferenceProperty($entity, $name, $values = array(), $langcode = NULL, $options = array()) {
    $values = is_array($values) ? $values : array();
    if (!empty($values)) {
      $values = $this->propertyReferences2ReferenceIds($name, $values);
    }
    $cur_ids = $this->getMultiValueProperty($entity, $name, NULL, $langcode, $options);
    $del_ids = array_diff($cur_ids, $values);

    if (!empty($del_ids)) {
      $this->deleteMultiReferenceValues($entity, $name, $del_ids);
    }
  }

  /**
   * Gets an entity multi value property.
   *
   * @param \EntityToolbox $entity
   *   The entity for which to set the value.
   * @param string         $name
   *   The property name.
   * @param int            $delta
   *   The values to set.
   * @param null|string    $langcode
   *   The language code.
   * @param array          $options
   *   The get options.
   *
   * @return array
   */
  public function getMultiValueProperty($entity, $name, $delta = NULL, $langcode = NULL, $options = array()) {
    $property = $this->toolboxInfo['properties'][$name];
    if (!empty($property['reference'])) {
      return isset($delta) ? $this->getMultiValueReferenceProperty($entity, $name, $delta, $langcode, $options)[$delta] : $this->getMultiValueReferenceProperty($entity, $name, $delta, $langcode, $options);
    }
    else {
      //@todo handle serialized multi values
      return array();
    }
  }

  /**
   * Gets an entity multi value reference property.
   *
   * @param \EntityToolbox $entity
   *   The entity for which to set the value.
   * @param string         $name
   *   The property name.
   * @param int            $delta
   *   The values to set.
   * @param null|string    $langcode
   *   The language code.
   * @param array          $options
   *   The get options.
   *
   * @return \Entity[]
   */
  public function getMultiValueReferenceProperty($entity, $name, $delta = NULL, $langcode = NULL, $options = array()) {
    list($self_id_key, $self_id, $rel_table, $ref_rel_key) = $this->relationExtractInfo($entity, $name);
    $query   = db_select($rel_table, 'e')
      ->fields('e', array())
      ->condition('e.' . $self_id_key, $self_id)
      ->execute();
    $results = array();
    while ($result = $query->fetchAssoc()) {
      $results[] = $result[$ref_rel_key];
    }

    return $results;
  }

  /**
   * Build breadcrumbs.
   */
  public function buildBreadCrumbs() {
  }

}