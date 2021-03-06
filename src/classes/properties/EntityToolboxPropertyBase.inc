<?php

/**
 * Class EntityToolboxPropertyBase
 */
class EntityToolboxPropertyBase {

  use EntityToolboxCommonTraits, TemplatingTraits;

  /**
   * @access protected
   * @var string $defaultType
   */
  protected $defaultType;
  /**
   * @access protected
   * @var string $defaultPhpType
   */
  protected $defaultPhpType;
  /**
   * @var string $defaultDrupalType
   */
  protected $defaultDrupalType;
  /**
   * @var string $defaultName
   */
  protected $defaultName;
  /**
   * @var string $defaultReference
   */
  protected $defaultReference;
  /**
   * @var string $defaultLabel
   */
  protected $defaultLabel;
  /**
   * @var string $defaultDescription
   */
  protected $defaultDescription;
  /**
   * @var boolean $defaultMultiple
   */
  protected $defaultMultiple;
  /**
   * @var boolean $defaultHasRevisions
   */
  protected $defaultHasRevisions;
  /**
   * @var boolean $defaultHasTranslations
   */
  protected $defaultHasTranslations;
  /**
   * @var boolean $defaultRequired
   */
  protected $defaultRequired;
  /**
   * @var boolean $defaultSerialize
   */
  protected $defaultSerialize;
  /**
   * @var string $defaultKey
   */
  protected $defaultKey;
  /**
   * @var array $defaultTables
   */
  protected $defaultTables = array();
  /**
   * @var array $defaultSchemasFieldsName
   */
  protected $defaultSchemasFieldsName = array();
  /**
   * @var array $defaultSchemasFields
   */
  protected $defaultSchemasFields = array();
  /**
   * @var array $defaultForms
   */
  protected $defaultForms = array();
  /**
   * @var array $defaultCallbacks
   */
  protected $defaultCallbacks = array();
  /**
   * @var boolean $defaultIsUnique
   */
  protected $defaultIsUnique;
  /**
   * @var boolean $defaultViewsExpose
   */
  protected $defaultViewsExpose;
  /**
   * @var array $defaultValue
   */
  protected $defaultValue = array();
  /**
   * @var boolean $defaultChildrenInherit
   */
  protected $defaultChildrenInherit;
  /**
   * @var integer $defaultWeight
   */
  protected $defaultWeight;

  /**
   * EntityToolboxPropertyBase constructor.
   *
   * @param array $args
   *   The args to set.
   */
  public function __construct(array $args = array()) {
    $args = array_replace_recursive(
      array(
        'self_extract_marker'         => 'default',
        'self_replace_marker'         => 'property_default',
        'default_type'                => 'base',
        'default_required'            => FALSE,
        'default_reference'           => '',
        'default_name'                => '',
        'default_label'               => '',
        'default_description'         => '',
        'default_multiple'            => FALSE,
        'default_has_revisions'       => TRUE,
        'default_has_translations'    => TRUE,
        'default_serialize'           => FALSE,
        'default_key'                 => '',
        'default_tables'              => array(
          'relation'          => '%toolbox_info_entity_type%_has_%reference_info_machine_name_plural%',
          'relation_revision' => '%toolbox_info_entity_type%_has_%reference_info_machine_name_plural%_revision',
        ),
        'default_schemas_fields_name' => array(
          'base'              => '',
          'revision'          => '%property_default_schemas_fields_name_base%',
          'relation'          => '',
          'relation_revision' => '%property_default_schemas_fields_name_relation%',
        ),
        'default_schemas_fields'      => array(
          'base'              => array(
            'description' => '',
            'type'        => '',
            'serialize'   => FALSE,
            'size'        => NULL,
            'not null'    => '',
            'default'     => '',
            'length'      => '',
            'unsigned'    => NULL,
            'precision'   => NULL,
            'scale'       => NULL,
            'binary'      => FALSE,
          ),
          'revision'          => array(
            'description' => '',
            'type'        => '',
            'serialize'   => FALSE,
            'size'        => NULL,
            'not null'    => '',
            'default'     => '',
            'length'      => '',
            'unsigned'    => NULL,
            'precision'   => NULL,
            'scale'       => NULL,
            'binary'      => FALSE,
          ),
          'relation'          => array(
            'description' => '',
            'type'        => '',
            'serialize'   => FALSE,
            'size'        => NULL,
            'not null'    => '',
            'default'     => '',
            'length'      => '',
            'unsigned'    => NULL,
            'precision'   => NULL,
            'scale'       => NULL,
            'binary'      => FALSE,
          ),
          'relation_revision' => array(
            'description' => '',
            'type'        => '',
            'serialize'   => FALSE,
            'size'        => NULL,
            'not null'    => '',
            'default'     => '',
            'length'      => '',
            'unsigned'    => NULL,
            'precision'   => NULL,
            'scale'       => NULL,
            'binary'      => FALSE,
          ),
        ),
        'default_callbacks'           => array(
          'getter'       => NULL,
          'setter'       => NULL,
          'validation'   => NULL,
          'permission'   => NULL,
          'default'      => NULL,
          'options_list' => NULL,
        ),
        'default_forms'               => array(
          'edit'   => array(
            'expose'   => TRUE,
            'widget'   => 'textfield',
            'required' => TRUE,
            'cols'     => '',
            'rows'     => '',
            'weight'   => '',
          ),
          'delete' => array(
            'expose'   => FALSE,
            'widget'   => '',
            'required' => FALSE,
            'cols'     => '',
            'rows'     => '',
            'weight'   => '',
          ),
          'clone'  => array(
            'expose'   => FALSE,
            'widget'   => '',
            'required' => FALSE,
            'cols'     => '',
            'rows'     => '',
            'weight'   => '',
          ),
          'export' => array(
            'expose'   => FALSE,
            'widget'   => '',
            'required' => FALSE,
            'cols'     => '',
            'rows'     => '',
            'weight'   => '',
          ),
          'inline' => array(
            'expose'   => FALSE,
            'widget'   => '',
            'required' => FALSE,
            'cols'     => '',
            'rows'     => '',
            'weight'   => '',
          ),
        ),
        'default_is_unique'           => FALSE,
        'default_views_expose'        => TRUE,
        'default_value'               => array(
          'default'    => '',
          'min_length' => NULL,
          'max_length' => NULL,
          'floor'      => NULL,
          'ceil'       => NULL,
          'filter'     => array(),
          'forbid'     => array(),
          'source'     => '',
        ),
        'default_children_inherit'    => FALSE,
        'default_weight'              => 0,
      ),
      $args
    );

    foreach ($args as $argName => $arg) {
      $this->__set($argName, $arg);
    }
  }

  /**
   * @return mixed
   */
  public function getDefaultType() {
    return $this->defaultType;
  }

  /**
   * @param mixed $defaultType
   */
  public function setDefaultType($defaultType) {
    $this->defaultType = $defaultType;
  }

  /**
   * @return mixed
   */
  public function getDefaultPhpType() {
    return $this->defaultPhpType;
  }

  /**
   * @param mixed $defaultPhpType
   */
  public function setDefaultPhpType($defaultPhpType) {
    $this->defaultPhpType = $defaultPhpType;
  }

  /**
   * @return mixed
   */
  public function getDefaultDrupalType() {
    return $this->defaultDrupalType;
  }

  /**
   * @param mixed $defaultDrupalType
   */
  public function setDefaultDrupalType($defaultDrupalType) {
    $this->defaultDrupalType = $defaultDrupalType;
  }

  /**
   * @return mixed
   */
  public function getDefaultName() {
    return $this->defaultName;
  }

  /**
   * @param mixed $defaultName
   */
  public function setDefaultName($defaultName) {
    $this->defaultName = $defaultName;
  }

  /**
   * @return mixed
   */
  public function getDefaultReference() {
    return $this->defaultReference;
  }

  /**
   * @param mixed $defaultReference
   */
  public function setDefaultReference($defaultReference) {
    $this->defaultReference = $defaultReference;
  }

  /**
   * @return mixed
   */
  public function getDefaultLabel() {
    return $this->defaultLabel;
  }

  /**
   * @param mixed $defaultLabel
   */
  public function setDefaultLabel($defaultLabel) {
    $this->defaultLabel = $defaultLabel;
  }

  /**
   * @return mixed
   */
  public function getDefaultDescription() {
    return $this->defaultDescription;
  }

  /**
   * @param mixed $defaultDescription
   */
  public function setDefaultDescription($defaultDescription) {
    $this->defaultDescription = $defaultDescription;
  }

  /**
   * @return mixed
   */
  public function getDefaultMultiple() {
    return $this->defaultMultiple;
  }

  /**
   * @param mixed $defaultMultiple
   */
  public function setDefaultMultiple($defaultMultiple) {
    $this->defaultMultiple = $defaultMultiple;
  }

  /**
   * @return mixed
   */
  public function getDefaultHasRevisions() {
    return $this->defaultHasRevisions;
  }

  /**
   * @param mixed $defaultHasRevisions
   */
  public function setDefaultHasRevisions($defaultHasRevisions) {
    $this->defaultHasRevisions = $defaultHasRevisions;
  }

  /**
   * @return mixed
   */
  public function getDefaultHasTranslations() {
    return $this->defaultHasTranslations;
  }

  /**
   * @param mixed $defaultHasTranslations
   */
  public function setDefaultHasTranslations($defaultHasTranslations) {
    $this->defaultHasTranslations = $defaultHasTranslations;
  }

  /**
   * @return mixed
   */
  public function getDefaultRequired() {
    return $this->defaultRequired;
  }

  /**
   * @param mixed $defaultRequired
   */
  public function setDefaultRequired($defaultRequired) {
    $this->defaultRequired = $defaultRequired;
  }

  /**
   * @return mixed
   */
  public function getDefaultSerialize() {
    return $this->defaultSerialize;
  }

  /**
   * @param mixed $defaultSerialize
   */
  public function setDefaultSerialize($defaultSerialize) {
    $this->defaultSerialize = $defaultSerialize;
  }

  /**
   * @return mixed
   */
  public function getDefaultKey() {
    return $this->defaultKey;
  }

  /**
   * @param mixed $defaultKey
   */
  public function setDefaultKey($defaultKey) {
    $this->defaultKey = $defaultKey;
  }

  /**
   * @return array
   */
  public function getDefaultTables() {
    return $this->defaultTables;
  }

  /**
   * @param array $defaultTables
   */
  public function setDefaultTables($defaultTables) {
    $this->defaultTables = $defaultTables;
  }

  /**
   * @return array
   */
  public function getDefaultSchemasFieldsName() {
    return $this->defaultSchemasFieldsName;
  }

  /**
   * @param array $defaultSchemasFieldsName
   */
  public function setDefaultSchemasFieldsName($defaultSchemasFieldsName) {
    $this->defaultSchemasFieldsName = $defaultSchemasFieldsName;
  }

  /**
   * @return array
   */
  public function getDefaultSchemasFields() {
    return $this->defaultSchemasFields;
  }

  /**
   * @param array $defaultSchemasFields
   */
  public function setDefaultSchemasFields($defaultSchemasFields) {
    $this->defaultSchemasFields = $defaultSchemasFields;
  }

  /**
   * @return array
   */
  public function getDefaultForms() {
    return $this->defaultForms;
  }

  /**
   * @param array $defaultForms
   */
  public function setDefaultForms($defaultForms) {
    $this->defaultForms = $defaultForms;
  }

  /**
   * @return array
   */
  public function getDefaultCallbacks() {
    return $this->defaultCallbacks;
  }

  /**
   * @param array $defaultCallbacks
   */
  public function setDefaultCallbacks($defaultCallbacks) {
    $this->defaultCallbacks = $defaultCallbacks;
  }

  /**
   * @return mixed
   */
  public function getDefaultIsUnique() {
    return $this->defaultIsUnique;
  }

  /**
   * @param mixed $defaultIsUnique
   */
  public function setDefaultIsUnique($defaultIsUnique) {
    $this->defaultIsUnique = $defaultIsUnique;
  }

  /**
   * @return mixed
   */
  public function getDefaultViewsExpose() {
    return $this->defaultViewsExpose;
  }

  /**
   * @param mixed $defaultViewsExpose
   */
  public function setDefaultViewsExpose($defaultViewsExpose) {
    $this->defaultViewsExpose = $defaultViewsExpose;
  }

  /**
   * @return array
   */
  public function getDefaultValue() {
    return $this->defaultValue;
  }

  /**
   * @param array $defaultValue
   */
  public function setDefaultValue($defaultValue) {
    $this->defaultValue = $defaultValue;
  }

  /**
   * @return mixed
   */
  public function getDefaultChildrenInherit() {
    return $this->defaultChildrenInherit;
  }

  /**
   * @param mixed $defaultChildrenInherit
   */
  public function setDefaultChildrenInherit($defaultChildrenInherit) {
    $this->defaultChildrenInherit = $defaultChildrenInherit;
  }

  /**
   * @return mixed
   */
  public function getDefaultWeight() {
    return $this->defaultWeight;
  }

  /**
   * @param mixed $defaultWeight
   */
  public function setDefaultWeight($defaultWeight) {
    $this->defaultWeight = $defaultWeight;
  }

  /**
   * Indicates if the property is a reference.
   *
   * @return boolean
   */
  public function isPropertyReference() {
    return FALSE;
  }
}