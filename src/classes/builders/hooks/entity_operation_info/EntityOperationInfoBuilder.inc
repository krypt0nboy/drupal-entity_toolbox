<?php

require_once ENTITY_TOOLBOX_PATH . '/src/classes/builders/hook_keys/entity_operation_info/EntityOperationInfoOperationBuilder.inc';

/**
 * Class EntityOperationInfoBuilder
 */
class EntityOperationInfoBuilder extends EntityToolboxDependentHookBuilder {

  /**
   * @var array $operations
   *   The operations within toolboxInfo.
   */
  protected $operations = array();

  /** {{@inheritdoc}} */
  public function __construct(array $args = array()) {
    $args = $this->mergeConstructArgs(array(
      'hook' => 'entity_operation_info',
    ), $args);
    parent::__construct($args);
  }

  /**
   * @return array
   */
  public function getOperations() {
    return $this->operations;
  }

  /**
   * @param array $operations
   */
  public function setOperations($operations) {
    $this->operations = $operations;
  }

  /** {{@inheritdoc}} */
  public function build() {
    $this->setPhase(self::PHZ__BUILD);
    $this->preBuild();
    foreach (array_keys($this->structure) as $op) {
      $this->setKeyBuild($op, $this->buildOperation($op));
    }
    $this->validateBuild();
    $this->postBuild();

    return $this->deliver();
  }

  /** {{@inheritdoc}} */
  protected function genDynamicStructure() {
    $this->setOperations($this->toolboxInfo['operations']);
    $structure          = array();
    $keyBuildersClasses = array();
    $keyBuildersArgs    = array();
    foreach ($this->operations as $op => $params) {
      $structure[$op]          = array(
        'type'   => 'array',
        'method' => 'buildOperation',
      );
      $keyBuildersClasses[$op] = 'EntityOperationInfoOperationBuilder';
      $keyBuildersArgs[$op]    = array(
        'operation_name' => $op,
      );
    }
    $this->__set('key_builders_classes', $keyBuildersClasses);
    $this->__set('key_builders_args', $keyBuildersArgs);

    return $structure;
  }

  /**
   * @param $operationName
   * @return mixed
   */
  protected function buildOperation($operationName) {
    return $this->getKeyBuilder($operationName)->build();
  }
}