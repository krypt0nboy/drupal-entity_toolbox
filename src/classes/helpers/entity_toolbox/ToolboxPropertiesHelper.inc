<?php

require_once 'EntityToolboxHelperBase.inc';

/**
 * Class ToolboxPropertiesHelper
 */
class ToolboxPropertiesHelper extends EntityToolboxHelperBase {
  /**
   * @var array $requiredSchemaPropertyTypes
   *   The property types required for this schema type.
   */
  protected $requiredSchemaPropertyTypes = array();
  /** {{@inheritdoc}} */
  const ALIAS = 'properties';
  /**
   * @var string $idProperty
   */
  protected $idProperty;
  /**
   * @var string $revisionProperty
   */
  protected $revisionProperty;
  /**
   * @var string $nameProperty
   */
  protected $nameProperty;

  /** {{@inheritdoc}} */
  public function __construct(array $args = array()) {
    $args = $this->mergeConstructArgs(array(
      'required_schema_property_types' => array(
        'entity_id'          => TRUE,
        'entity_revision_id' => TRUE,
        'translation_set_id' => TRUE,
      )
    ), $args);
    parent::__construct($args);
  }

  /**
   * @return array
   */
  public function getRequiredSchemaPropertyTypes() {
    return $this->requiredSchemaPropertyTypes;
  }

  /**
   * @param array $requiredSchemaPropertyTypes
   */
  public function setRequiredSchemaPropertyTypes($requiredSchemaPropertyTypes) {
    $this->requiredSchemaPropertyTypes = $requiredSchemaPropertyTypes;
  }

  /**
   * @return string
   */
  public function getIdProperty() {
    return $this->idProperty;
  }

  /**
   * @param string $idProperty
   */
  public function setIdProperty($idProperty) {
    $this->idProperty = $idProperty;
  }

  /**
   * @return string
   */
  public function getNameProperty() {
    return $this->nameProperty;
  }

  /**
   * @param string $nameProperty
   */
  public function setNameProperty($nameProperty) {
    $this->nameProperty = $nameProperty;
  }

  /**
   * @return string
   */
  public function getRevisionProperty() {
    return $this->revisionProperty;
  }

  /**
   * @param string $revisionProperty
   */
  public function setRevisionProperty($revisionProperty) {
    $this->revisionProperty = $revisionProperty;
  }

  /**
   * Returns the required properties.
   *
   * @return array
   */
  public function requiredPropertiesGet() {
    $props = $this->alias();

    return array_keys(array_filter_by_value($props, TRUE, 'required'));
  }

  /**
   * Returns the exposed properties for a given form type.
   *
   * @param string $type
   *   The form type ("edit", "delete", etc...).
   *
   * @return array
   */
  public function entityFormExposedPropertiesGet($type) {
    $props  = $this->alias();
    $expose = array();
    foreach ($props as $prop => $info) {
      $formParams = $info['forms'][$type];
      if ($formParams['expose']) {
        $expose[$prop] = $info;
      }
    }

    return $expose;
  }

  /**
   * Returns a property.
   *
   * @param string $name
   *   The property name.
   *
   * @return array
   */
  public function propGet($name) {
    $props = $this->alias();

    return $props[$name];
  }

  /*** ISSERS ***/

  /**
   * Indicates if a property is required.
   *
   * @param string $name
   *   The property name.
   *
   * @return bool
   */
  public function propIsRequired($name) {
    $prop = $this->propGet($name);

    return $prop['required'];
  }

  /**
   * Indicates if a property is required.
   *
   * @param string $name
   *   The property name.
   *
   * @return bool
   */
  public function propIsMulti($name) {
    $prop = $this->propGet($name);

    return (bool) $prop['multiple'];
  }

  /**
   * Indicates if a property is required.
   *
   * @param string $name
   *   The property name.
   *
   * @return bool
   */
  public function propIsSerialized($name) {
    $prop = $this->propGet($name);

    return (bool) $prop['serialize'];
  }

  /**
   * Indicates if a property is required.
   *
   * @param string $name
   *   The property name.
   *
   * @return bool
   */
  public function propIsIdProperty($name) {
    $prop = $this->propGet($name);

    return $prop['type'] == 'entity_id';
  }

  /**
   * Indicates if a property is required.
   *
   * @param string $name
   *   The property name.
   *
   * @return bool
   */
  public function propIsRevisionProperty($name) {
    $prop = $this->propGet($name);

    return $prop['type'] == 'entity_revision_id';
  }

  /**
   * Indicates if a property is required.
   *
   * @param string $name
   *   The property name.
   *
   * @return bool
   */
  public function propIsBundleProperty($name) {
    $prop = $this->propGet($name);

    return $prop['type'] == 'entity_bundle';
  }

  /**
   * Indicates if a property is required.
   *
   * @param string $name
   *   The property name.
   *
   * @return bool
   */
  public function propIsReference($name) {
    $prop = $this->propGet($name);

    return !empty($prop['reference']);
  }

  /**
   * Indicates if a property is required.
   *
   * @param string $name
   *   The property name.
   *
   * @return bool
   */
  public function propIsSelfReference($name) {
    $ref = $this->propGetReference($name);
    if (isset($ref)) {
      return $ref == $this->entityType;
    }

    return FALSE;
  }

  /**
   * Indicates if a property is required.
   *
   * @param string $name
   *   The property name.
   *
   * @return bool
   */
  public function propIsParent($name) {
    $type = $this->propGetType($name);

    return $type == 'parent' || $type == 'self_parent';
  }

  /**
   * Indicates if a property is a self parent.
   *
   * @param string $name
   *   The property name.
   *
   * @return bool
   */
  public function propIsSelfParent($name) {
    if ($this->propIsParent($name)) {
      $ref = $this->propGetReference($name);

      return $ref ==  $this->getDataHolder()->getEntityType();
    }
    else {
      return FALSE;
    }
  }

  /**
   * Indicates if a property is required for schemas.
   *
   * @param string $name
   *   The property name.
   *
   * @return bool
   */
  public function propIsRequiredForSchema($name) {
    $prop = $this->propGet($name);
    $type = $prop['type'];

    return in_array($type, array_keys($this->requiredSchemaPropertyTypes));
  }

  /*** ISSERS END ***/

  /*** HASSERS ***/

  /**
   * Indicates if a property has revisions.
   *
   * @param string $name
   *   The property name.
   *
   * @return bool
   */
  public function propHasRevisions($name) {
    $prop = $this->propGet($name);

    return $prop['has_revisions'];
  }

  /**
   * Indicates if a property has translations.
   *
   * @param string $name
   *   The property name.
   *
   * @return bool
   */
  public function propHasTranslations($name) {
    $prop = $this->propGet($name);

    return $prop['has_translations'];
  }

  /**
   * Indicates if a property has a key.
   *
   * @param string $name
   *   The property name.
   *
   * @return bool
   */
  public function propHasKey($name) {
    $prop = $this->propGet($name);

    return !empty($prop['key']);
  }

  /**
   * Indicates if a property has a field for a given schema type.
   *
   * @param string $name
   *   The property name.
   * @param string $schemaType
   *   The schema type.
   *
   * @return bool
   */
  public function propHasSchemaFieldName($name, $schemaType) {
    $prop = $this->propGet($name);

    return !empty($prop['schemas_fields_name'][$schemaType]);
  }

  /**
   * Indicates if a property has a default value.
   *
   * @param string $name
   *   The property name.
   * @param string $callbackType
   *   The callback type.
   *
   * @return bool
   */
  public function propHasCallback($name, $callbackType) {
    $prop = $this->propGet($name);

    return !empty($prop['callbacks'][$callbackType]);
  }

  /**
   * Indicates if a property has a default value.
   *
   * @param string $name
   *   The property name.
   *
   * @return bool
   */
  public function propHasDefaultValueCallback($name) {
    return $this->propHasCallback($name, 'default');
  }

  /**
   * Indicates if a property has a static default value.
   *
   * @param string $name
   *   The property name.
   *
   * @return bool
   */
  public function propHasDefaultValueStatic($name) {
    $prop = $this->propGet($name);

    return !is_null($prop['value']['default']);
  }

  /**
   * Indicates if a property has a default value.
   *
   * @param string $name
   *   The property name.
   *
   * @return bool
   */
  public function propHasDefaultValue($name) {
    $has = $this->propHasDefaultValueStatic($name);
    if (!$has) {
      $has = $this->propHasDefaultValueCallback($name);
    }

    return $has;
  }

  /*** HASSERS END ***/

  /*** GETTERS ***/

  /**
   * Returns the toolbox property type.
   *
   * @param string $name
   *   The property name.
   *
   * @return string
   */
  public function propGetType($name) {
    $prop = $this->propGet($name);

    return $prop['type'];
  }

  /**
   * Returns the property reference.
   *
   * @param string $name
   *   The property name.
   *
   * @return null|string
   */
  public function propGetReference($name) {
    $prop = $this->propGet($name);
    if ($this->propIsReference($name)) {

      return $prop['reference'];
    }
    else {
      return NULL;
    }
  }

  /**
   * Returns the property key.
   *
   * @param string $name
   *   The property name.
   *
   * @return null|string
   */
  public function propGetKey($name) {
    $prop = $this->propGet($name);
    if ($this->propHasKey($name)) {

      return $prop['key'];
    }
    else {
      return NULL;
    }
  }

  /**
   * Returns the property relation table name.
   *
   * @param string $name
   *   The property name.
   *
   * @return null|string
   */
  public function propGetRelationTable($name) {
    $prop = $this->propGet($name);
    if (!empty($prop['tables']['relation'])) {
      return $prop['tables']['relation'];
    }
  }

  /**
   * Returns the property relation_revision table name.
   *
   * @param string $name
   *   The property name.
   *
   * @return null|string
   */
  public function propGetRelationRevisionTable($name) {
    $prop = $this->propGet($name);
    if (!empty($prop['tables']['relation_revision'])) {
      return $prop['tables']['relation_revision'];
    }
  }

  /**
   * Returns the property schema field name for a given schema type.
   *
   * @param string $name
   *   The property name.
   * @param string $schemaType
   *   The schema type.
   *
   * @return null|string
   */
  public function propGetSchemaFieldName($name, $schemaType) {
    $hasField = $this->propHasSchemaFieldName($name, $schemaType);
    if ($hasField) {
      $prop = $this->propGet($name);

      return $prop['schemas_fields_name'][$schemaType];
    }
  }

  /**
   * Returns the property schema field for a given schema type.
   *
   * @param string $name
   *   The property name.
   * @param string $schemaType
   *   The schema type.
   *
   * @return null|string
   */
  public function propGetSchemaField($name, $schemaType) {
    $prop = $this->propGet($name);

    return $prop['schemas_fields'][$schemaType];
  }

  /**
   * Returns the property callback.
   *
   * @param string $name
   *   The property name.
   * @param string $callbackType
   *   The callback type.
   *
   * @return null|string
   */
  public function propGetCallback($name, $callbackType) {
    if ($this->propHasCallback($name, $callbackType)) {
      $prop = $this->propGet($name);

      return $prop['callbacks'][$callbackType];
    }
  }

  /**
   * Returns the property default value callback.
   *
   * @param string $name
   *   The property name.
   *
   * @return null|string
   */
  public function propGetDefaultValueCallback($name) {
    return $this->propGetCallback($name, 'default');
  }

  /**
   * Returns the property default value.
   *
   * @param string $name
   *   The property name.
   *
   * @return mixed
   */
  public function propGetDefaultValue($name) {
    if ($this->propHasDefaultValueCallback($name)) {
      $callback = $this->propGetDefaultValueCallback($name);

      return $callback();
    }
    else {
      if ($this->propHasDefaultValueStatic($name)) {
        return $this->propGetDefaultValueStatic($name);
      }
    }
  }

  public function propGetValueParam($name, $param) {
    if (($prop = $this->propGet($name)) !== NULL) {
      return $prop['value'][$param];
    }
  }

  /**
   * Returns the property default static value.
   *
   * @param string $name
   *   The property name.
   *
   * @return mixed
   */
  public function propGetDefaultValueStatic($name) {
    $prop = $this->propGet($name);

    return $prop['value']['default'];
  }

  /**
   * Returns the id property name.
   *
   * @return string
   */
  public function propsGetIdProperty() {
    if ($this->__isset('id_property')) {
      return $this->idProperty;
    }
    else {
      $props = $this->alias();

      return reset(array_filter_by_value($props, 'entity_id', 'type'));
    }
  }

  /*** GETTERS END ***/

  /*** MISC ***/

  /**
   * Counts the properties.
   *
   * @return int
   */
  public function countProperties() {
    return parent::count();
  }

  /*** MISC END ***/
}