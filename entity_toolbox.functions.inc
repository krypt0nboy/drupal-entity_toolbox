<?php

entity_toolbox_include('format');

/**
 * Require once an entity toolbox file.
 *
 * @param        $file
 *   The filename to include.
 * @param string $prefix
 *   The prefix of the file.
 *   Defaults to "entity_toolbox".
 * @param bool   $dir
 *   The entity toolbox directory where the file is located or FALSE for root directory.
 */
function entity_toolbox_include($file, $prefix = 'entity_toolbox', $dir = FALSE) {
  static $_used = array();
  $directory = '/' . ($dir ? $dir . '/' : '');
  if (!isset($_used[$dir ? $dir : ''][$file])) {
	require_once DRUPAL_ROOT . '/' . ENTITY_TOOLBOX_PATH . "$directory$prefix.$file.inc";
	$_used[$dir ? $dir : ''][$file] = TRUE;
  }
}

/**
 * Auto includes controller classes files for declared entities.
 * @param $module
 */
function module_entities_class_autoload($module) {
  $module_path     = drupal_get_path('module', $module);
  $module_entities = module_invoke($module, 'entity_toolbox_entity_info');
  foreach ($module_entities as $entity_type => $info) {
	require_once DRUPAL_ROOT . '/' . $module_path . '/includes/' . entity_type2entity_class($entity_type) . '.inc';
  }
}

/**
 * Converts a toolbox property type into an entity property type.
 *
 * @param $type
 *   The type provided in hook_entity_toolbox_entity_info().
 *
 * @return string
 *
 * @see hook_entity_toolbox_entity_info().
 */
function toolbox_property_type2entity_property_type($type) {
  switch ($type) {
	case 'id':
	  return 'integer';
	  break;
	case 'text_field':
	case 'text':
	  return 'text';
	  break;
	case 'machine_name':
	case 'token':
	  return 'token';
	  break;
	case 'serialized':
	  return 'text';
	  break;
	case 'integer':
	  return 'integer';
	  break;
	case 'float':
	  return 'decimal';
	  break;
	case 'decimal':
	  return 'decimal';
	  break;
	case 'timestamp':
	  return 'integer';
	  break;
	case 'date':
	  return 'date';
	  break;
	case 'duration':
	  return 'duration';
	  break;
	case 'boolean':
	  return 'boolean';
	  break;
	case 'parent':
	case 'reference':
	  return FALSE;
	  break;
  }
}

/**
 * @param $widget
 * @return string
 */
function toolbox_property_field_widget2field_type($widget) {
  switch ($widget) {
	case 'textfield':
	  return 'textfield';
	  break;
	case 'list':
	  return 'select';
	  break;
	case 'checkbox':
	  return 'checkbox';
	  break;
	case 'autocomplete':
	  return 'textfield';
	  break;
	case 'machine_name':
	  return 'machine_name';
	  break;
	case 'textarea':
	  break;
	default:
	  return 'textfield';
	  break;
  }
}

/**
 * Indicates whether the property is a reference.
 *
 * @param $type
 *   The property type.
 *
 * @return bool
 * @see hook_entity_toolbox_entity_info().
 *
 */
function is_property_reference($type) {
  return ($type == 'parent') || ($type == 'reference');
}

/**
 * Indicates if the reference type is the same as the entity type.
 *
 * @param $self
 *   The entity type.
 * @param $reference
 *   The reference type.
 *
 * @return bool
 */
function is_reference_self($self, $reference) {
  return $self == $reference;
}

/**
 * Indicates if the reference can be a multiple value.
 *
 * @param $property
 *   The property info.
 *
 * @return bool
 */
function is_reference_multiple($property) {
  return isset($property['multiple']) && $property['multiple'];
}

/**
 * Indicates if the property requires a relation table to be created.
 *
 * @param $property
 *   The property info.
 *
 * @return bool
 */
function requires_relation_table($property) {
  $type = $property['type'];
  return (($type == 'parent') || ($type == 'reference')) && !empty($property['relation_table']) && (isset($property['multiple']) && $property['multiple']);
}

/**
 * @param $form
 * @param $form_state
 * @param $entity_type
 * @param $entity
 * @param $name
 * @return string
 */
function property_default_value(&$form, &$form_state, $entity_type, $entity, $name) {
  return '';
}