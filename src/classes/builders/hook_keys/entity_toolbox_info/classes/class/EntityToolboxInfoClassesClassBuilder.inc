<?php

/**
 * Class ClassBuilder
 */
class EntityToolboxInfoClassesClassBuilder extends EntityToolboxInfoKeyBuilder {
  /**
   * @var string $classType
   *   The class type.
   */
  protected $classType;
  /**
   * @var array $classTypeInfo
   *   The class type info.
   */
  protected $classTypeInfo = array();
  /**
   * @var string $baseClass
   *   The base class name.
   */
  protected $baseClass;

  /** {{@inheritdoc}} */
  public function __construct(array $args = array()) {
    $args = $this->mergeConstructArgs(
      array(
        'parent_key'  => 'classes',
        'model_class' => 'EntityToolboxInfoClassesClassModel',
      ),
      $args
    );
    parent::__construct($args);
  }

  /**
   * @return string
   */
  public function getClassType() {
    return $this->classType;
  }

  /**
   * @param string $classType
   */
  public function setClassType($classType) {
    $this->classType = $classType;
  }

  /**
   * @return array
   */
  public function getClassTypeInfo() {
    return $this->classTypeInfo;
  }

  /**
   * @param array $classTypeInfo
   */
  public function setClassTypeInfo($classTypeInfo) {
    $this->classTypeInfo = $classTypeInfo;
  }

  /**
   * @return string
   */
  public function getBaseClass() {
    return $this->baseClass;
  }

  /**
   * @param string $baseClass
   */
  public function setBaseClass($baseClass) {
    $this->baseClass = $baseClass;
  }

  /** {{@inheritdoc}} */
  protected function autoSet() {
    parent::autoSet();
    $this->autoSetBaseClass();
    $this->autoSetClassTypeInfo();
  }

  /**
   * Auto sets classTypeInfo.
   */
  protected function autoSetClassTypeInfo() {
    $classTypeInfo = entity_toolbox_class_type_get_info($this->classType);
    $this->__set('class_type_info', $classTypeInfo);
  }

  /**
   * Auto sets classBaseClass.
   */
  protected function autoSetBaseClass() {
    $baseClass = str2camelcase($this->entityType);
    $this->__set('base_class', $baseClass);
  }

  /** {{@inheritdoc}} */
  protected function initTemplateVars(array $vars = array()) {
    $vars += $this->flattenExtract('', 'class_default_', $this->classTypeInfo['default']);
    $vars += array('class_base_class' => $this->baseClass);
    parent::initTemplateVars($vars);
  }
}