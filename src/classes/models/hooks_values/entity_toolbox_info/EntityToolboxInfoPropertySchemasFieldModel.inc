<?php

/**
 * Class EntityToolboxInfoPropertySchemasField
 */
class EntityToolboxInfoPropertySchemasFieldModel extends EntityToolboxInfoRecursiveModel {
  /**
   * @var string $schemaType
   */
  protected $schemaType;

  /** {{@inheritdoc}} */
  public function __construct(array $args = array()) {
    $schemaFieldModelInstance = new DrupalSchemaFieldModel(array('schema_type' => $args['schema_type']));
    $struct                   = $schemaFieldModelInstance->getStructure();
    $args                     = $this->mergeConstructArgs(
      array(
        'structure' => $struct
      ),
      $args
    );
    parent::__construct($args);
  }

  /** {{@inheritdoc}} */
  protected function reprocessStructure($structure) {
    $replace = '%property_default_schemas_fields_' . $this->constructArgs['schema_type'] . '_';
    foreach ($structure as $key => $params) {
      if (!empty($params['default'])) {
        $default = &$structure[$key]['default'];
        $default = str_replace('%property_schemas_fields_' . $this->constructArgs['schema_type'] . '_', $replace, $default);
      }
    }

    return $structure;
  }

  protected function setStructure($structure) {
    $reprocessed = $this->reprocessStructure($structure);
    parent::setStructure($reprocessed);
  }

  /**
   * @return mixed
   */
  public function getSchemaType() {
    return $this->schemaType;
  }

  /**
   * @param mixed $schemaType
   */
  public function setSchemaType($schemaType) {
    $this->schemaType = $schemaType;
  }
}