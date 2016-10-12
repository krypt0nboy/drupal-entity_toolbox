<?php

/**
 * Class KeysValueBuilder
 */
class EntityToolboxInfoKeysBuilder extends EntityToolboxInfoKeyBuilder {
  /** {{@inheritdoc}} */
  public function __construct(array $args = array()) {
    $args = $this->mergeConstructArgs(
      array(
        'self_key'                  => 'keys',
        'up_structure_before_build' => TRUE,
      ),
      $args
    );
    parent::__construct($args);
  }

  /** {{@inheritdoc}} */
  public function genDynamicStructure() {
    $structure = array();
    //@todo add check method to ensure key has been built
    $hookBuilder = $this->getHookBuilder();
    $hlp         = $hookBuilder->hlpr();
    if ($hlp->keyHasBuild('properties')) {
      $properties = $hookBuilder->getBuild('properties');
      foreach ($properties as $property => $info) {
        $schema_field_name = $info['schemas_fields_name']['base'];
        if (!empty($info['key'])) {
          if (!empty($info['reference'])) {
            $structure[$property] = array(
              'type'    => 'string',
              'default' => $schema_field_name,
            );
          }
          else {
            $structure[$info['key']] = array(
              'type'    => 'string',
              'default' => $schema_field_name,
            );
          }
//          if (($info['type'] == 'bundle') || ($info['type'] == 'entity_id')) {
//            $structure[$info['key']] = array(
//              'type'    => 'string',
//              'default' => $schema_field_name,
//            );
//          }
//          else {
//            $structure[$property] = array(
//              'type'    => 'string',
//              'default' => $info['key'],
//            );
//          }
        }
      }
    }

    return $structure;
  }
}