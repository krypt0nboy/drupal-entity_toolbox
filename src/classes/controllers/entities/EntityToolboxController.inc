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
   * @return string
   */
  public function getEntityType() {
    return $this->entityType;
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
    $values += array(
      'is_new' => TRUE
    );
    $entity = parent::create($values);

    return $entity;
  }

  /**
   * Converts an entity 2 an array.
   *
   * @param object $entity
   *   The entity to convert.
   *
   * @return array
   */
  public function entity2array($entity) {
    $arrayEntity = array();
    foreach (get_class_vars(get_class($entity)) as $var => $v) {
      $arrayEntity[$var] = $entity->$var;
    }

    return $arrayEntity;
  }

  /** {{@inheritdoc}} */
  protected function saveRevision($entity) {
    // Convert the entity into an array as it might not have the same properties
    // as the entity, it is just a raw structure.
    $record = $this->entity2array($entity);
    // File fields assumes we are using $entity->revision instead of
    // $entity->is_new_revision, so we also support it and make sure it's set to
    // the same value.
    $entity->is_new_revision             = !empty($entity->is_new_revision) || !empty($entity->revision) || $entity->is_new;
    $entity->revision                    = $entity->is_new_revision;
    $entity->{$this->defaultRevisionKey} = !empty($entity->{$this->defaultRevisionKey}) || $entity->is_new;


    // When saving a new revision, set any existing revision ID to NULL so as to
    // ensure that a new revision will actually be created.
    if ($entity->is_new_revision && isset($record[$this->revisionKey])) {
      $record[$this->revisionKey] = NULL;
    }

    if ($entity->is_new_revision) {
      drupal_write_record($this->revisionTable, $record);
      $update_default_revision = $entity->{$this->defaultRevisionKey};
    }
    else {
      drupal_write_record($this->revisionTable, $record, $this->revisionKey);
      // @todo: Fix original entity to be of the same revision and check whether
      // the default revision key has been set.
      $update_default_revision = $entity->{$this->defaultRevisionKey} && $entity->{$this->revisionKey} != $entity->original->{$this->revisionKey};
    }
    // Make sure to update the new revision key for the entity.
    $entity->{$this->revisionKey} = $record[$this->revisionKey];

    // Mark this revision as the default one.
    if ($update_default_revision) {
      db_update($this->entityInfo['base table'])
        ->fields(array($this->revisionKey => $record[$this->revisionKey]))
        ->condition($this->idKey, $entity->{$this->idKey})
        ->execute();
    }

    return $entity->is_new_revision ? SAVED_NEW : SAVED_UPDATED;
  }

  /** {{@inheritdoc}} */
  protected function attachLoad(&$queried_entities, $revision_id = FALSE) {
    parent::attachLoad($queried_entities, $revision_id);
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
    $helper   = $this->getPropsHelper();
    $id       = $entity->{$this->idKey};
    $relTable = $helper->propGetRelationTable($name);
    $field    = $helper->propGetSchemaFieldName($name, 'relation');

    return array(
      $this->idKey,
      $id,
      $relTable,
      $field,
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
    $refField      = $helper->propGetSchemaFieldName($name, 'relation');
    $inserted      = array();
    foreach ($add_ids as $add_id) {
      $inserted[] = db_insert($table)
        ->fields(
          array(
            $entityIdField => $entityId,
            $refField      => $add_id,
            'vid'          => $entity->vid
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
    $keys   = $this->entityInfo['entity keys'];
    $flip   = array_flip($keys);
    $helper = $this->getPropsHelper();
    foreach ($this->properties as $name => $info) {
      $key = $helper->propHasKey($name);
      if ($key && $helper->propHasDefaultValue($name)) {
        $key  = $this->keyGet($name);
        if (!empty($keys[$key])) {
          $key_ = $keys[$key];
        }
        else {
          if (!empty($flip[$key])) {
            $key_ = $flip[$key];
          }
          else {
            if (!empty($keys[$name])) {
              $key_ = $keys[$name];
            }
            else {
              if (!empty($flip[$name])) {
                $key_ = $flip[$name];
              }
            }
          }
        }
        $values[$key_] = $this->propertyDefaultValue($name);
      }
    }

    return $values;
  }

  /**
   * @param $property
   * @return mixed
   */
  public function keyGet($property) {
    $helper = $this->getPropsHelper();
    $key    = $helper->propGetKey($property);

    return $key;
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

  /**
   * Overriding the buildContent function to add entity specific fields.
   *
   * @param object $entity
   *   Entity.
   * @param string $view_mode
   *   View mode.
   * @param string $langcode
   *   Language code.
   * @param array  $content
   *   Content.
   *
   * @return array
   *   The content built.
   */
  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {
    $content = parent::buildContent($entity, $view_mode, $langcode, $content);
//    $wrapper = entity_metadata_wrapper($this->entityType, $entity);
//    foreach ($wrapper as $name => $property) {
//      $info = $property->info();
//      $content[$name] = array(
//        '#label_hidden'   => FALSE,
//        '#label'          => $info['label'],
//        '#entity_wrapped' => $wrapper,
//        '#theme'          => 'entity_property',
//        '#property_name'  => $name,
//        '#entity_type'    => $this->entityType,
//      );
//    }

    return $content;
  }

}