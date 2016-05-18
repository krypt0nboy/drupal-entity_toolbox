<?php

/**
 * @TODO : add reverse_entity_get_info() to work with entities that were not declared through entity_toolbox.
 */


function entity_toolbox_cache_clear_all() {
  $cache_keys = array(
	'entity_toolbox_entity_info',
	'entity_toolbox_module_entities_list',
	'entities_bundles',
	'entity_toolbox_current_versions',
	'entities_inheritable_properties',
	'entities_inherited_properties',
	'entities_parent_properties',
	'entities_parent_entities'
  );
  foreach ($cache_keys as $cache_key) {
	drupal_static_reset($cache_key);
	cache_clear_all($cache_key, 'cache');
  }
}

/**
 * Gets the info declared through entity_toolbox.
 * @TODO : clean the function code.
 *
 * @param null $entity_type
 *   The machine_name of the entity for which to get the info or NULL to return an array for all the entities.
 *
 * @param bool $update_N
 *
 * @see hook_entity_toolbox_entity_info().
 * @see hook_entity_toolbox_entity_info_alter().
 * @see hook_entity_toolbox_ENTITY_TYPE_properties_update_N().
 *
 * @return array
 */
function entity_toolbox_entity_get_info($entity_type = NULL, $update_N = FALSE, $reset = FALSE) {
  /**
   * We cache entity_toolbox_entity_info since its used very often.
   */
  static $drupal_static_fast;
  if (!isset($drupal_static_fast)) {
	$drupal_static_fast['entity_toolbox_entity_info'] = &drupal_static(__FUNCTION__);
  }

  $entity_toolbox_entity_info = &$drupal_static_fast['entity_toolbox_entity_info'];
//  if (empty($entity_toolbox_entity_info)) {
//	if ($cache = cache_get('entity_toolbox_entity_info') && !$reset) {
//	  $entity_toolbox_entity_info = $cache->data;
//	}
//	else {
  entity_toolbox_include('format');
  $data = module_invoke_all('entity_toolbox_entity_info');
  drupal_alter('entity_toolbox_entity_info', $data);
  foreach ($data as $type => $info) {
	foreach ($info['properties'] as $name => $property) {
	  $schema_field = !empty($property['schema field']) ? $property['schema field'] : $name;
	  switch ($property['type']) {
		case 'parent':
		case 'reference':
		  if (!is_reference_multiple($property)) {
			$data[$type]['properties'][$name]['schema'] += array(
			  'type' => 'int',
			);
		  }
		  else {
			unset($data[$type]['properties'][$name]['schema']);
		  }
		  break;
		case 'id':
		  if (empty($info['id_property'])) {
			$data[$type]['id_property'] = $name;
		  }
		  $data[$type]['properties'][$name]['schema'] += array(
			'description' => 'Primary Key: Identifier for a ' . $type,
			'type'        => 'serial',
			'not null'    => TRUE,
			'unsigned'    => TRUE,
		  );
		  break;
		case 'boolean':
		  $data[$type]['properties'][$name]['schema'] += array(
			'type'     => 'int',
			'size'     => 'tiny',
			'not null' => TRUE,
			'default'  => PROPERTY_BOOLEAN_DEFAULT_DEFAULT,
		  );
		  break;
		case 'machine_name':
		case 'token':
		  if ($property['key'] == 'bundle') {
			$data[$type]['bundle_property'] = $name;
		  }
		  $data[$type]['properties'][$name]['schema'] += array(
			'type'     => 'varchar',
			'length'   => PROPERTY_TOKEN_DEFAULT_LENGTH,
			'not null' => TRUE,
			'default'  => '',
		  );
		  break;
		case 'serialized':
		  $data[$type]['properties'][$name]['schema'] += array(
			'type'      => 'blob',
			'serialize' => TRUE,
			'size'      => PROPERTY_SERIALIZED_DEFAULT_SIZE
		  );
		  break;
		case 'decimal':
		  $data[$type]['properties'][$name]['schema'] += array(
			'type'      => 'numeric',
			'size'      => 'normal',
			'precision' => PROPERTY_DECIMAL_DEFAULT_PRECISION,
			'scale'     => PROPERTY_DECIMAL_DEFAULT_SCALE,
			'not null'  => TRUE,
			'default'   => PROPERTY_DECIMAL_DEFAULT_DEFAULT,
		  );
		  break;
		case 'integer':
		  $data[$type]['properties'][$name]['schema'] += array(
			'type'     => 'int',
			'not null' => TRUE,
			'size'     => PROPERTY_INTEGER_DEFAULT_SIZE,
			'default'  => PROPERTY_INTEGER_DEFAULT_DEFAULT,
		  );
		  break;
		case 'float':
		  $data[$type]['properties'][$name]['schema'] += array(
			'type'     => 'float',
			'not null' => TRUE,
			'size'     => PROPERTY_FLOAT_DEFAULT_SIZE,
			'default'  => PROPERTY_FLOAT_DEFAULT_DEFAULT,
		  );
		  break;
		case 'struct':
		  break;
		case 'timestamp':
		  $data[$type]['properties'][$name]['schema'] += array(
			'type'     => 'int',
			'not null' => TRUE,
			'default'  => 0,
		  );
		  break;
		case 'date':
		  $data[$type]['properties'][$name]['schema'] += array(
			'type'       => 'datetime',
			'mysql_type' => 'datetime',
			'not null'   => FALSE,
		  );
		  break;
		case 'duration':
		  $data[$type]['properties'][$name]['schema'] += array(
			'type'     => 'int',
			'not null' => FALSE,
			'default'  => 0
		  );
		  break;
		case 'text':
		  $data[$type]['properties'][$name]['schema'] += array(
			'type'     => 'text',
			'size'     => PROPERTY_TEXT_DEFAULT_SIZE,
			'not null' => TRUE,
			'default'  => '',
		  );
		  break;
		case 'text_field':
		  $data[$type]['properties'][$name]['schema'] += array(
			'type'     => 'varchar',
			'length'   => PROPERTY_TEXT_FIELD_DEFAULT_LENGTH,
			'not null' => TRUE,
			'default'  => '',
		  );
		  break;
	  }
	}
  }
  $entity_toolbox_entity_info = $data;
//	cache_set('entity_toolbox_entity_info', $entity_toolbox_entity_info);
//	}
//  }
  if (isset($entity_type)) {
	return !empty($entity_toolbox_entity_info[$entity_type]) ? $entity_toolbox_entity_info[$entity_type] : NULL;
  }
  else {
	return $entity_toolbox_entity_info;
  }
}

/**
 * Returns a list of entities declared through entity_toolbox.
 * @param null $get
 * @return array
 */
function entity_toolbox_entity_get_list($get = NULL) {

  $cache_key = 'entity_toolbox_entity_list';
  if (isset($get)) {
	if ($get == GET_FIELDABLE) {
	  $cache_key .= ':' . GET_FIELDABLE;
	}
	else {
	  if ($get == GET_NOT_FIELDABLE) {
		$cache_key .= ':' . GET_NOT_FIELDABLE;
	  }
	}
  }

  static $drupal_static_fast;
  if (!isset($drupal_static_fast)) {
	$drupal_static_fast['entity_toolbox_entity_list'] = &drupal_static(__FUNCTION__);
  }
  $entity_toolbox_entity_list = &$drupal_static_fast['entity_toolbox_entity_list'];
  if (empty($entity_toolbox_entity_list)) {
	if ($cache = cache_get($cache_key)) {
	  $entity_toolbox_entity_list = $cache->data;
	}
	else {
	  $toolbox_info  = entity_toolbox_entity_get_info();
	  $fieldable     = array();
	  $not_fieldable = array();
	  $all           = array();
	  foreach ($toolbox_info as $type => $info) {
		if ($info['fieldable']) {
		  $fieldable[] = $type;
		}
		else {
		  $not_fieldable[] = $type;
		}
	  }
	  $all = array_merge($fieldable, $not_fieldable);
	  cache_set('entity_toolbox_entity_list', $all);
	  cache_set('entity_toolbox_entity_list:' . GET_FIELDABLE, $fieldable);
	  cache_set('entity_toolbox_entity_list:' . GET_NOT_FIELDABLE, $not_fieldable);
	  if (isset($get)) {
		if ($get == GET_FIELDABLE) {
		  $entity_toolbox_entity_list = $fieldable;
		}
		else {
		  if ($get == GET_NOT_FIELDABLE) {
			$entity_toolbox_entity_list = $not_fieldable;
		  }
		}
	  }
	  else {
		$entity_toolbox_entity_list = $all;
	  }
	}
  }

  return $entity_toolbox_entity_list;
}

/**
 * Gets the info declared through entity_toolbox.
 * @TODO : clean the function code.
 *
 * @param null $entity_type
 *   The machine_name of the entity for which to get the info or NULL to return an array for all the entities.
 *
 * @param bool $update_N
 *
 * @see hook_entity_toolbox_entity_info().
 * @see hook_entity_toolbox_entity_info_alter().
 * @see hook_entity_toolbox_ENTITY_TYPE_properties_update_N().
 *
 * @return array
 */
function entity_toolbox_entity_get_info__($entity_type = NULL, &$update_N = FALSE) {
  entity_toolbox_include('format');

  /**
   * We cache entity_toolbox_entity_info since its used very often.
   */
  static $drupal_static_fast;
  if (!isset($drupal_static_fast)) {
	$drupal_static_fast['entity_toolbox_entity_info'] = &drupal_static(__FUNCTION__);
  }

  $entity_toolbox_entity_info = &$drupal_static_fast['entity_toolbox_entity_info'];
  if (empty($entity_toolbox_entity_info)) {
	if ($cache = cache_get('entity_toolbox_entity_info')) {
	  $entity_toolbox_entity_info = $cache->data;
	}
	else {
	  $data                       = module_invoke_all('entity_toolbox_entity_info');
	  $entity_toolbox_entity_info = $data;
	  foreach ($entity_toolbox_entity_info as $type => $info) {
		if ($info['is_fieldable'] && empty($info['is_bundle_of'])) {
		  $entity_toolbox_entity_info[$type] += array(
			'bundle_entity' => $type . '_type',
		  );
		  $entity_toolbox_entity_info[$type]['properties'] += entity_toolbox_default_properties($type);
		}
		else {
		  $entity_toolbox_entity_info[$type] += array(
			'is_bundle_of' => preg_replace('/_type/', '', $type),
		  );
		  $entity_toolbox_entity_info[$type]['properties'] += entity_toolbox_default_properties($type, FALSE);
		}
		$entity_toolbox_entity_info[$type] = $info;
	  }
	  $data = $entity_toolbox_entity_info;
	  drupal_alter('entity_toolbox_entity_info', $data);
	  cache_set('entity_toolbox_entity_info', $entity_toolbox_entity_info);
	}
	if ($entity_type) {
	  if (!$update_N) {
		return !empty($entity_toolbox_entity_info[$entity_type]) ? $entity_toolbox_entity_info[$entity_type] : array();
	  }
	  else {
		$version = intval(entity_toolbox_entity_get_current_version($entity_type));
		$version = $version == 0 ? 7001 : $version + 1;
		for ($v = 7001; $v <= $version; $v++) {
		  $updates = module_invoke_all('entity_toolbox_' . $entity_type . '_properties_update_' . $v);
		  $entity_toolbox_entity_info[$entity_type]['properties'] += $updates;
		}
		return !empty($entity_toolbox_entity_info[$entity_type]) ? $entity_toolbox_entity_info[$entity_type] : array();
	  }
	}
	else {
	  if (!$update_N) {
		return $entity_toolbox_entity_info;
	  }
	  else {
		foreach ($entity_toolbox_entity_info as $type => $info) {
		  $version = entity_toolbox_entity_get_current_version($type);
		  $version = $version == 0 ? 7001 : $version + 1;
		  for ($v = 7001; $v <= $version; $v++) {
			$updates = module_invoke_all('entity_toolbox_' . $type . '_properties_update_' . $v);
			$entity_toolbox_entity_info[$type]['properties'] += $updates;
		  }
		}
		return $entity_toolbox_entity_info;
	  }
	}
  }
  else {
	if ($entity_type) {
	  if (!$update_N) {
		return !empty($entity_toolbox_entity_info[$entity_type]) ? $entity_toolbox_entity_info[$entity_type] : array();
	  }
	  else {
		$version = intval(entity_toolbox_entity_get_current_version($entity_type));
		$version = $version == 0 ? 7001 : $version + 1;
		for ($v = 7001; $v <= $version; $v++) {
		  $updates = module_invoke_all('entity_toolbox_' . $entity_type . '_properties_update_' . $v);
		  $entity_toolbox_entity_info[$entity_type]['properties'] += $updates;
		}
		return !empty($entity_toolbox_entity_info[$entity_type]) ? $entity_toolbox_entity_info[$entity_type] : array();
	  }
	}
	else {
	  if (!$update_N) {
		return $entity_toolbox_entity_info;
	  }
	  else {
		foreach ($entity_toolbox_entity_info as $type => $info) {
		  $version = entity_toolbox_entity_get_current_version($type);
		  $version = $version == 0 ? 7001 : $version + 1;
		  for ($v = 7001; $v <= $version; $v++) {
			$updates = module_invoke_all('entity_toolbox_' . $type . '_properties_update_' . $v);
			$entity_toolbox_entity_info[$type]['properties'] += $updates;
		  }
		}
		return $entity_toolbox_entity_info;
	  }
	}
  }
}

/**
 * Clear the cache for entity_toolbox_entity_info.
 */
function entity_toolbox_info_cache_clear() {
  drupal_static_reset('entity_toolbox_entity_get_info');
  cache_clear_all('entity_toolbox_entity_info', 'cache');
}

/**
 * Returns the id key of an entity_type.
 * @param null $entity_type
 * @return array
 */
function entity_toolbox_entity_get_id_key($entity_type = NULL) {
  /**
   * We cache entity_toolbox_entity_info since its used very often.
   */
  static $drupal_static_fast;
  if (!isset($drupal_static_fast)) {
	$drupal_static_fast['entity_info_id_keys'] = &drupal_static(__FUNCTION__);
  }

  $entity_info_id_keys = &$drupal_static_fast['entity_info_id_keys'];
  if (empty($entity_info_id_keys)) {
	if ($cache = cache_get('entity_info_id_keys')) {
	  $entity_info_id_keys = $cache->data;
	}
	else {
	  $keys        = array();
	  $entity_info = entity_get_info();
	  foreach ($entity_info as $type => $info) {
		$keys[$type] = $info['entity keys']['id'];
	  }
	  $entity_info_id_keys = $keys;
	  cache_set('entity_info_id_keys', $keys);
	}
  }
  return isset($entity_type) ? $entity_info_id_keys[$entity_type] : $entity_info_id_keys;
}

/**
 * Returns the list of entities managed by a module.
 *
 * @param      $module
 *   The machine_name of the module for which to get the info or NULL to return an array for all the modules.
 *
 * @return array
 */
function entity_toolbox_module_entities_get_list($module = NULL) {
  /**
   * We cache entity_toolbox_module_entities_get_list since its used very often.
   */
  static $drupal_static_fast;
  if (!isset($drupal_static_fast)) {
	$drupal_static_fast['entity_toolbox_module_entities_list'] = &drupal_static(__FUNCTION__);
  }
  $entity_toolbox_module_entities_list = &$drupal_static_fast['entity_toolbox_module_entities_list'];
  if (empty($entity_toolbox_module_entities_list)) {
	if ($cache = cache_get('entity_toolbox_module_entities_list')) {
	  $entity_toolbox_module_entities_list = $cache->data;
	}
	else {
	  $list = array();
	  $info = entity_toolbox_entity_get_info();
	  foreach ($info as $entity_type => $i) {
		$module          = $i['module'];
		$list[$module][] = $entity_type;
	  }
	  $entity_toolbox_module_entities_list = $list;
	  cache_set('entity_toolbox_module_entities_list', $entity_toolbox_module_entities_list);
	}
  }
  if (isset($module)) {
	return $entity_toolbox_module_entities_list[$module];
  }
  else {
	return $entity_toolbox_module_entities_list;
  }
}

/**
 * Clear the cache for entity_toolbox_entity_info.
 */
function entity_toolbox_module_entities_list_cache_clear() {
  drupal_static_reset('entity_toolbox_module_entities_list');
  cache_clear_all('entity_toolbox_module_entities_list', 'cache');
}

/**
 * Returns the entity groups info declared.
 *
 * @param null $group
 *   The entity group name or NULL to get all.
 * @return array
 *
 * @see hook_entity_toolbox_entity_group_info().
 */
function entity_toolbox_entity_group_info($group = NULL) {
  /**
   * We cache entity_toolbox_entity_info since its used very often.
   */
  static $drupal_static_fast;
  if (!isset($drupal_static_fast)) {
	$drupal_static_fast['entity_toolbox_entity_groups'] = &drupal_static(__FUNCTION__);
  }

  $entity_toolbox_entity_groups = &$drupal_static_fast['entity_toolbox_entity_groups'];
  if (empty($entity_toolbox_entity_groups)) {
	if ($cache = cache_get('entity_toolbox_entity_groups')) {
	  $entity_toolbox_entity_groups = $cache->data;
	}
	else {
	  $data = module_invoke_all('entity_toolbox_entity_group_info');
	  drupal_alter('entity_toolbox_entity_group_info', $data);
	  $entity_toolbox_entity_groups = $data;
	  cache_set('entity_toolbox_entity_groups', $entity_toolbox_entity_groups);
	}
  }

  return isset($group) ? $entity_toolbox_entity_groups[$group] : $entity_toolbox_entity_groups;

}

function entity_toolbox_entity_group_entities_info($group = NULL) {
  /**
   * We cache entity_toolbox_entity_info since its used very often.
   */
  static $drupal_static_fast;
  if (!isset($drupal_static_fast)) {
	$drupal_static_fast['entity_toolbox_entity_group_entities'] = &drupal_static(__FUNCTION__);
  }

  $entity_toolbox_entity_group_entities = &$drupal_static_fast['entity_toolbox_entity_group_entities'];
  if (empty($entity_toolbox_entity_group_entities)) {
	if ($cache = cache_get('entity_toolbox_entity_group_entities')) {
	  $entity_toolbox_entity_group_entities = $cache->data;
	}
	else {
	  $group_info   = entity_toolbox_entity_group_info();
	  $toolbox_info = entity_toolbox_entity_get_info();
	  $groups       = array();
	  foreach ($group_info as $group_ => $info) {
		$groups[$group_] = array();
	  }
	  foreach ($toolbox_info as $type => $info) {
		if (!empty($info['group']) && array_key_exists($info['group'], $groups)) {
		  $groups[$info['group']][] = $type;
		}
	  }
	  $entity_toolbox_entity_group_entities = $groups;
	  cache_set('entity_toolbox_entity_group_entities', $entity_toolbox_entity_group_entities);
	}
  }

  return isset($group) ? $entity_toolbox_entity_group_entities[$group] : $entity_toolbox_entity_group_entities;
}

/**
 * Returns the bundles of an entity_type.
 *
 * @param null $entity_type
 *  The machine_name of the entity for which to get the info or NULL to return an array for all the entities.
 *
 * @return array
 */
function entity_toolbox_entity_get_bundles($entity_type = NULL) {
  static $drupal_static_fast;
  if (!isset($drupal_static_fast)) {
	$drupal_static_fast['entities_bundles'] = &drupal_static(__FUNCTION__);
  }

  $entities_bundles = &$drupal_static_fast['entities_bundles'];
  if (empty($entities_bundles)) {
	if ($cache = cache_get('entities_bundles')) {
	  $entities_bundles = $cache->data;
	}
	else {
	  $list         = array();
	  $toolbox_info = entity_toolbox_entity_get_info();
	  foreach ($toolbox_info as $type => $info) {
		if (!empty($info['bundle of'])) {
		  $list[$info['bundle of']][] = $type;
		}
	  }
	  $entities_bundles = $list;
	  cache_set('entities_bundles', $entities_bundles);
	}
  }

  if (isset($entity_type)) {
	return $entities_bundles[$entity_type];
  }
  else {
	return $entities_bundles;
  }
}

/**
 * Updates the properties version of an entity_type.
 *
 * @param      $entity_type
 * @param null $current_version
 */
function entity_toolbox_entity_version_update($entity_type, $current_version = NULL) {
  $version = isset($current_version) ? $current_version : entity_toolbox_entity_get_current_version($entity_type);
  $query   = db_update('entity_toolbox_versions')
	->fields(array('current_version' => $version + 1))
	->condition('entity_type', $entity_type)
	->execute();
}

/**
 * Returns the current entity_type properties version.
 *
 * @param null $entity_type
 *   The machine_name of the entity for which to get the info or NULL to return an array for all the entities.
 *
 * @return integer
 */
function entity_toolbox_entity_get_current_version($entity_type = NULL) {
  static $drupal_static_fast;
  if (!isset($drupal_static_fast)) {
	$drupal_static_fast['entity_toolbox_current_versions'] = &drupal_static(__FUNCTION__);
  }
  $entity_toolbox_current_versions = &$drupal_static_fast['entity_toolbox_current_versions'];
  if (empty($entity_toolbox_current_versions)) {
	if ($cache = cache_get('entity_toolbox_current_versions')) {
	  $entity_toolbox_current_versions = $cache->data;
	}
	else {
	  $toolbox_info = entity_toolbox_entity_get_info();
	  $query        = db_select('entity_toolbox_versions', 'e')
		->fields('e', array('entity_type', 'current_version'));
	  $results      = $query->execute();
	  $versions     = array();
	  while ($result = $results->fetchAssoc()) {
		$versions[$result['entity_type']] = $result['current_version'];
	  }
	  foreach ($toolbox_info as $type => $type_info) {
		if (!array_key_exists($type, $versions)) {
		  $query_2 = db_insert('entity_toolbox_versions')
			->fields(array(
			  'entity_type'     => $type,
			  'current_version' => 0
			))
			->execute();
		}
	  }
	  $entity_toolbox_current_versions = $versions;
	  cache_set('entity_toolbox_current_versions', $entity_toolbox_current_versions);
	}
  }
  if (isset($entity_type)) {
	return $entity_toolbox_current_versions[$entity_type];
  }
  else {
	return $entity_toolbox_current_versions;
  }
}

/**
 * Returns an array of an entity_type properties.
 *
 * @param null $entity_type
 *   The entity type machine_name or NULL to return all entities properties.
 *
 * @param bool $update_N
 */
function entity_toolbox_entity_properties_get_info($entity_type = NULL, $update_N = FALSE) {
  $properties = array();
  foreach (entity_toolbox_entity_get_info() as $type => $info) {
	foreach ($info['properties'] as $name => $property) {

	}
  }
}

/**
 * Returns an entity_type properties update info range.
 *
 * @param      $entity_type
 *   The entity_type for which to get the updates.
 * @param null $start
 *   The update to start getting the info from or NULL to start at 7001 (first update).
 * @param null $end
 *   The update to end getting the info from or NULL to end at infinite (the loop will stop as soon as the hook_call returns an empty result).
 *
 * @return array
 *
 * @see hook_entity_toolbox_ENTITY_TYPE_properties_update_N().
 */
function entity_toolbox_entity_get_properties_update_info($entity_type, $start = NULL, $end = NULL) {
  $updates = array();
  $start   = isset($start) ? $start : 7001;
  if (!isset($end)) {
	$count = $start;
	$keep  = TRUE;
	while ($keep) {
	  $update = module_invoke_all('entity_toolbox_' . $entity_type . '_properties_update_' . $count);
	  $keep   = !empty($update) ? TRUE : FALSE;
	  if ($keep) {
		$updates[$count] = $update;
		$count++;
	  }
	}
  }
  else {
	$keep = TRUE;
	for ($i = $start; $i <= $end && $keep == TRUE; $i++) {
	  $update = module_invoke_all('entity_toolbox_' . $entity_type . '_properties_update_' . $i);
	  $keep   = !empty($update) ? TRUE : FALSE;
	  if ($keep) {
		$updates[$i] = $update;
	  }
	}
  }

  return $updates;
}

/**
 * @param null $entity_type
 * @param null $name
 */
function entity_toolbox_entity_property_settings_get_info($entity_type = NULL, $name = NULL) {

}

/**
 * Returns the system path of an entity_type.
 *
 * @param $entity_type
 * @param $alias
 *
 * @return string
 */
function entity_toolbox_entity_get_path($entity_type, $alias) {
  $toolbox_info = entity_toolbox_entity_get_info($entity_type);

  if ($alias == 'root') {
	return $toolbox_info['paths']['root'];
  }
  else {
	$aliases = array_keys($toolbox_info['paths']);
	$search  = array_search($alias, $aliases);
	$path    = $toolbox_info['paths']['root'];
	for ($i = 1; $i <= $search; $i++) {
	  $path .= '/' . $toolbox_info['paths'][$aliases[$i]];
	}
	return $path;
  }
}

/**
 * @TODO : add_cache
 * @TODO : add_bundles
 * @TODO : add dynamic properties getters/setters.
 *
 * Returns the data for hook_entity_info().
 *
 * @param $entity_type
 *
 * @return mixed
 *
 * @see hook_entity_toolbox_entity_info().
 */
function entity_toolbox_entity_info_build($entity_type) {
  entity_toolbox_include('format');
  $toolbox_info        = entity_toolbox_entity_get_info($entity_type);
  $build[$entity_type] = array(
	'label'            => t(entity_type2entity_label($entity_type)),
	'plural label'     => t(entity_type2entity_label_plural($entity_type)),
	'entity class'     => entity_type2entity_class($entity_type),
	'controller class' => entity_type2entity_controller_class($entity_type),
	'base table'       => $toolbox_info['base table'],
	'fieldable'        => $toolbox_info['fieldable'],
	'entity keys'      => array(),
	'access callback'  => $entity_type . '_access',
	'module'           => $toolbox_info['module'],
	'admin ui'         => array(
	  'path'             => $toolbox_info['paths']['root'] . '/' . $toolbox_info['paths']['admin'],
	  'file'             => $toolbox_info['module'] . '.' . $entity_type . '.admin.inc',
	  'controller class' => entity_type2entity_ui_controller_class($entity_type),
	),
  );
  if ($toolbox_info['fieldable']) {
	$build[$entity_type]['label callback']            = 'entity_class_label';
	$build[$entity_type]['uri callback']              = 'entity_class_uri';
	$build[$entity_type]['creation callback']         = $entity_type . '_create';
	$build[$entity_type]['bundles']                   = array();
	$build[$entity_type]['bundle keys']               = array(
	  'bundle' => $toolbox_info['bundle_property'],
	);
	$build[$entity_type]['admin ui']['menu wildcard'] = '%' . $entity_type;
//	$build[$entity_type]['inline entity form']['controller'] = entity_type2entity_class($entity_type) . 'InlineEntityFormController';
	$build[$entity_type]['inline entity form']['controller'] = 'EntityToolboxEntityInlineEntityFormController';
  }
  else {
	$build[$entity_type]['exportable'] = TRUE;
	$build[$entity_type]['bundle of']  = $toolbox_info['bundle of'];
  }
  foreach ($toolbox_info['properties'] as $name => $property) {
	if (!empty($property['key'])) {
	  $build[$entity_type]['entity keys'][$property['key']] = $name;
	}
  }
  return $build;
}

/**
 * @param $module
 * @return array
 */
function entity_toolbox_module_entities_info_build($module) {
  $info = array();
  $list = entity_toolbox_module_entities_get_list($module);
  foreach ($list as $entity_type) {
	foreach (entity_toolbox_entity_info_build($entity_type) as $type => $spec) {
	  $info[$type] = $spec;
	}
  }
  return $info;
}

/**
 * @param $entity_type
 * @param $entity_info
 * @return mixed
 */
function entity_toolbox_entity_info_alter_build(&$entity_info, $entity_type) {
  entity_toolbox_include('extend');
  //@TODO : add check if table exists
  if ($entity_info[$entity_type]['fieldable']) {
	if (db_table_exists($entity_type . '_type')) {
	  foreach (entity_get_types($entity_type) as $bundle => $bundle_info) {
		$path                                                     = entity_toolbox_entity_get_path($entity_type . '_type', 'manage') . '/%' . $entity_type . '_type';
		$real_path                                                = entity_toolbox_entity_get_path($entity_type . '_type', 'manage') . '/' . $bundle;
		$entity_info[$entity_type]['bundles'][$bundle_info->type] = array(
		  'label' => $bundle_info->label,
		  'admin' => array(
			'path'             => $path,
			'real path'        => $real_path,
			'bundle argument'  => array_search($bundle_info->type, explode('/', entity_toolbox_entity_get_path($entity_type . '_type', 'manage') . '/' . $bundle_info->type)),
			'access arguments' => array("access administration pages"),
		  ),
		);
	  }
	}
  }
//  foreach (entity_toolbox_entity_get_properties_update_info($entity_type, NULL, entity_toolbox_entity_get_current_version($entity_type)) as $name => $update) {
//	$entity_info['entity keys'][$update['key']] = $name;
//  }

  return $entity_info;
}

/**
 * Returns the entity_type schema.
 *
 * @param $entity_type
 *
 * @TODO : set_cache
 *
 * @return array
 */
function entity_toolbox_entity_schema_build($entity_type) {
  entity_toolbox_include('format');
  $toolbox_info_all  = entity_toolbox_entity_get_info();
  $toolbox_info      = entity_toolbox_entity_get_info($entity_type);
  $schema            = array();
  $schema_properties = array();
  foreach ($toolbox_info['properties'] as $name => $property) {
	if (!empty($property['schema field']) || !empty($property['relation_table'])) {
	  $schema_properties[$name] = $property;
	}
  }
  if ($toolbox_info['fieldable']) {
	$schema[$toolbox_info['base table']] = array(
	  'description' => 'The base table for ' . $entity_type . ' entities.',
	  'fields'      => array(),
	  'primary key' => array(),
	  'unique keys' => array(),
	  'indexes'     => array(),
	);
	foreach ($schema_properties as $name => $schema_property) {
	  if (!empty($schema_property['schema field']) && empty($schema_property['relation_table'])) {
		$schema[$toolbox_info['base table']]['fields'][$schema_property['schema field']] = $schema_property['schema'];
		if (!empty($schema_property['unique']) && $schema_property['unique']) {
		  $schema[$toolbox_info['base table']]['unique keys'][$schema_property['schema field']] = array($schema_property['schema field']);
		}
		if ($schema_property['type'] == 'id') {
		  $schema[$toolbox_info['base table']]['primary key'] = array($schema_property['schema field']);
		}
		if ($name == $toolbox_info['bundle_property']) {
		  $schema[$toolbox_info['base table']]['indexes'][$schema_property['schema field']] = array($schema_property['schema field']);
		}
		if (($schema_property['type'] == 'reference') || ($schema_property['type'] == 'parent') && $schema_property['reference'] != $entity_type) {
		  if (empty($schema_property['multiple'])) {
			$r_type   = $schema_property['reference'];
			$r_info   = isset($toolbox_info_all[$r_type]) ? $toolbox_info_all[$r_type] : entity_get_info($r_type);
			$r_key_id = isset($r_info['id_property']) ? $r_info['id_property'] : $r_info['entity keys']['id'];
			$schema[$toolbox_info['base table']]['fields'][$schema_property['schema field']] += array(
			  'foreign keys' => array(
				$r_info['base table'] => array(
				  'table'   => $r_info['base table'],
				  'columns' => array($schema_property['schema field'] => $r_key_id),
				),
			  )
			);
		  }
		}
	  }
	  else {
		if (requires_relation_table($schema_property)) {
		  $r_type                                              = $schema_property['reference'];
		  $r_info                                              = isset($toolbox_info_all[$r_type]) ? $toolbox_info_all[$r_type] : entity_get_info($r_type);
		  $self_id_key                                         = $schema_property['relation_table']['keys']['self'];
		  $r_id_key                                            = $schema_property['relation_table']['keys']['reference'];
		  $schema[$schema_property['relation_table']['table']] = array(
			'description' => 'A relation table between a ' . $entity_type . ' and a ' . $r_type,
			'fields'      => array(
			  $self_id_key => array(
				'type'     => 'int',
				'not null' => TRUE,
				'default'  => 0
			  ),
			  $r_id_key    => array(
				'type'     => 'int',
				'not null' => TRUE,
				'default'  => 0
			  ),
			)
		  );
		}
	  }
	}
	if ($toolbox_info['has_revisions']) {
	  $revision_table                             = $entity_type . '_revision';
	  $schema[$toolbox_info['base table']]['vid'] = array(
		'description' => 'The current {' . $entity_type . '}.vid version identifier.',
		'type'        => 'int',
		'unsigned'    => TRUE,
		'not null'    => TRUE,
		'default'     => 0,
	  );
	  $schema[$revision_table]                    = array(
		'description' => 'Keeps track of ' . entity_type2entity_label_plural($entity_type),
		'fields'      => array(
		  $toolbox_info['id_property'] => array(
			'type'        => 'int',
			'unsigned'    => TRUE,
			'not null'    => TRUE,
			'default'     => 0,
			'description' => 'Unique ' . $entity_type . ' id (entity id).',
		  ),
		  'vid'                        => array(
			'description' => 'The current {' . $entity_type . '_revision}.vid version identifier.',
			'type'        => 'serial',
			'unsigned'    => TRUE,
			'not null'    => TRUE,
		  ),
		  'changed'                    => array(
			'description' => 'The Unix timestamp when the ' . $entity_type . ' was most recently saved.',
			'type'        => 'int',
			'not null'    => TRUE,
			'default'     => 0,
		  ),
		),
		'primary key' => array($toolbox_info['id_property'], 'vid'),
	  );
	}
  }
  else {
	echo '';
	$schema[$toolbox_info['base table']] = array(
	  'description' => 'Stores information about defined ' . $toolbox_info['bundle of'] . ' types.',
	  'fields'      => array(),
	  'primary key' => array(),
	  'unique keys' => array(),
	);
	foreach ($schema_properties as $name => $schema_property) {
	  if (empty($property['relation_table'])) {
		$schema[$toolbox_info['base table']]['fields'][$schema_property['schema field']] = $schema_property['schema'];
		if (!empty($schema_property['unique']) && $schema_property['unique']) {
		  $schema[$toolbox_info['base table']]['unique keys'][$schema_property['schema field']] = array($schema_property['schema field']);
		}
		if ($schema_property['type'] == 'id') {
		  $schema[$toolbox_info['base table']]['primary key'] = array($schema_property['schema field']);
		}
		if ($schema_property['type'] == 'reference' || $schema_property['type'] == 'parent' && $schema_property['reference'] != $entity_type) {
		  $r_type   = $schema_property['reference'];
		  $r_info   = isset($toolbox_info_all[$r_type]) ? $toolbox_info_all[$r_type] : entity_get_info($r_type);
		  $r_key_id = isset($r_info['id_property']) ? $r_info['id_property'] : $r_info['entity keys']['id'];
		  $schema[$toolbox_info['base table']]['fields'][$schema_property['schema field']] += array(
			'foreign keys' => array(
			  $r_info['base table'] => array(
				'table'   => $r_info['base table'],
				'columns' => array($schema_property['schema field'] => $r_key_id),
			  ),
			)
		  );
		}
	  }
	}
	if ($toolbox_info['exportable']) {
	  $schema[$toolbox_info['base table']]['fields'] += entity_exportable_schema_fields();
	}
  }

  if (empty($schema[$toolbox_info['base table']]['unique keys'])) {
	unset($schema[$toolbox_info['base table']]['unique keys']);
  }

  return $schema;
}

/**
 * @param $entity_type
 */
function entity_toolbox_entity_schema_alter_build($entity_type) {
  $alter = array(
	'add_field'  => array(),
	'drop_field' => array()
  );
  foreach (entity_toolbox_entity_get_properties_update_info($entity_type) as $name => $update) {

  }
}

/**
 * Returns the schema builds for every entity_type managed by a module.
 *
 * @param $module
 * @return array
 */
function entity_toolbox_module_entities_schema_build($module) {
  $schema = array();
  $list   = entity_toolbox_module_entities_get_list($module);
  foreach ($list as $entity_type) {
	foreach (entity_toolbox_entity_schema_build($entity_type) as $table => $spec) {
	  $schema[$table] = $spec;
	}
  }
  return $schema;
}

/**
 * Returns an entity_type properties build for hook_entity_properties_info().
 *
 * @param null $entity_type
 *  The machine_name of the entity for which to get the info or NULL to return an array for all the entities.
 *
 * @return array
 */
function entity_toolbox_entity_properties_info_build($entity_type = NULL) {
  /**
   * We cache entities_properties_info since its used very often.
   */
  static $drupal_static_fast;
  if (!isset($drupal_static_fast)) {
	$drupal_static_fast['entities_properties_info'] = &drupal_static(__FUNCTION__);
  }
  $entities_properties_info = &$drupal_static_fast['entities_properties_info'];
  if (empty($entities_properties_info)) {
	if ($cache = cache_get('entities_properties_info')) {
	  $entities_properties_info = $cache->data;
	}
	else {
	  $toolbox_info             = entity_toolbox_entity_get_info(NULL, FALSE);
	  $entities_properties_info = array();
	  foreach ($toolbox_info as $type => $type_info) {
		foreach ($type_info['properties'] as $name => $property) {
		  $p_type = toolbox_property_type2entity_property_type($property['type']);
		  if (!is_property_reference($property['type'])) {
			$entities_properties_info[$type][$name] = array(
			  'label'               => $property['title'],
			  'description'         => !empty($property['description']) ? $property['description'] : NULL,
			  'type'                => $p_type != FALSE ? $p_type : $property['reference'],
			  'options list'        => !empty($property['field']['options list']) ? $property['field']['options list'] : NULL,
			  'getter callback'     => !empty($property['callbacks']['getter']) ? $property['callbacks']['getter'] : NULL,
			  'setter callback'     => !empty($property['callbacks']['setter']) ? $property['callbacks']['setter'] : NULL,
			  'validation callback' => !empty($property['callbacks']['validation']) ? $property['callbacks']['validation'] : NULL,
			  'access callback'     => !empty($property['callbacks']['access']) ? $property['callbacks']['access'] : NULL,
			  'setter permission'   => !empty($property['permission']['setter']) ? $property['permission']['setter'] : NULL,
			  'schema field'        => !empty($property['schema field']) ? $property['schema field'] : NULL,
			  'required'            => !empty($property['required create']) ? $property['required create'] : NULL,
			  'entity views field'  => !empty($property['expose']['views']) ? $property['expose']['views'] : FALSE,
			  'translatable'        => !empty($property['translatable']) ? $property['translatable'] : FALSE,
			);
		  }
		  else {
			if (!requires_relation_table($property)) {
			  $entities_properties_info[$type][$name]                     = array(
				'label'               => $property['title'],
				'description'         => !empty($property['description']) ? $property['description'] : NULL,
				'type'                => $property['reference'],
				'getter callback'     => !empty($property['callbacks']['getter']) ? $property['callbacks']['getter'] : 'entity_get_attributes',
				'setter callback'     => !empty($property['callbacks']['setter']) ? $property['callbacks']['setter'] : 'entity_set_attributes',
				'validation callback' => !empty($property['callbacks']['validation']) ? $property['callbacks']['validation'] : NULL,
				'access callback'     => !empty($property['callbacks']['access']) ? $property['callbacks']['access'] : NULL,
				'setter permission'   => !empty($property['permission']['setter']) ? $property['permission']['setter'] : NULL,
				'entity views field'  => !empty($property['expose']['views']) ? $property['expose']['views'] : FALSE,
				'translatable'        => !empty($property['translatable']) ? $property['translatable'] : FALSE,
				'computed'            => TRUE,
				'required'            => !empty($property['required create']) ? $property['required create'] : NULL,
				'clear'               => array($property['schema field'])
			  );
			  $entities_properties_info[$type][$property['schema field']] = array(
				'label'             => $property['title'],
				'description'       => !empty($property['description']) ? $property['description'] : NULL,
				'type'              => 'integer',
				'setter callback'   => 'entity_property_verbatim_set',
				'setter permission' => 'access administration pages',
				'schema field'      => !empty($property['schema field']) ? $property['schema field'] : NULL,
				'clear'             => array($name)
			  );
			}
			else {
			  if (requires_relation_table($property)) {
				$tp                                     = 'list<' . $property['reference'] . '>';
				$entities_properties_info[$type][$name] = array(
				  'label'               => $property['title'],
				  'description'         => !empty($property['description']) ? $property['description'] : NULL,
				  'type'                => $tp,
				  'getter callback'     => !empty($property['callbacks']['getter']) ? $property['callbacks']['getter'] : 'entity_get_attributes',
				  'setter callback'     => !empty($property['callbacks']['setter']) ? $property['callbacks']['setter'] : 'entity_set_attributes',
				  'validation callback' => !empty($property['callbacks']['validation']) ? $property['callbacks']['validation'] : NULL,
				  'access callback'     => !empty($property['callbacks']['access']) ? $property['callbacks']['access'] : NULL,
				  'setter permission'   => !empty($property['permission']['setter']) ? $property['permission']['setter'] : NULL,
				  'entity views field'  => !empty($property['expose']['views']) ? $property['expose']['views'] : FALSE,
				  'translatable'        => !empty($property['translatable']) ? $property['translatable'] : FALSE,
				  'computed'            => TRUE,
				  'required'            => !empty($property['required create']) ? $property['required create'] : NULL,
				);
			  }
			}
		  }

		  foreach ($entities_properties_info[$type][$name] as $key => $item) {
			if ($item === NULL) {
			  unset($entities_properties_info[$type][$name][$key]);
			}
		  }
		}
	  }
	  cache_set('entities_properties_info', $entities_properties_info);
	}
  }

  if (isset($entity_type)) {
	return $entities_properties_info[$entity_type];
  }
  else {
	return $entities_properties_info;
  }
}

/**
 * @param $module
 * @return array
 */
function entity_toolbox_module_entities_properties_info_build($module) {
  $infos = array();
  $list  = entity_toolbox_module_entities_get_list($module);
  foreach ($list as $entity_type) {
	foreach (entity_toolbox_entity_properties_info_build($entity_type) as $property => $info) {
	  $infos[$entity_type][$property] = $info;
	}
  }
  return $infos;
}

/**
 * @param null $entity_type
 * @return array
 */
function entity_toolbox_entity_permissions_build($entity_type = NULL) {
  /**
   * We cache entities_permissions since its used very often.
   */
  static $drupal_static_fast;
  if (!isset($drupal_static_fast)) {
	$drupal_static_fast['entities_permissions'] = &drupal_static(__FUNCTION__);
  }
  $entities_permissions = &$drupal_static_fast['entities_permissions'];
  if (empty($entities_permissions)) {
	if ($cache = cache_get('entities_permissions')) {
	  $entities_permissions = $cache->data;
	}
	else {
	  entity_toolbox_include('format');
	  $toolbox_info = entity_toolbox_entity_get_info();
	  $permissions  = array();
	  foreach ($toolbox_info as $type => $type_info) {
		if ($type_info['fieldable']) {
		  $key                      = 'administer ' . entity_machine_name_single2plural($type);
		  $title                    = 'Administer ' . entity_machine_name_single2plural($type);
		  $description              = 'Edit and delete all ' . entity_machine_name_single2plural($type);
		  $permissions[$type][$key] = array(
			'title'       => t($title),
			'description' => t($description),
		  );
		}
		else {
		  $key                      = 'administer ' . entity_machine_name_single2plural($type);
		  $title                    = 'Administer ' . entity_machine_name_single2plural($type);
		  $description              = 'Create and delete fields for ' . entity_machine_name_single2plural($type) . ' and set their permissions.';
		  $permissions[$type][$key] = array(
			'title'       => t($title),
			'description' => t($description),
		  );
		}
	  }
	  $entities_permissions = $permissions;
	  cache_set('entities_permissions', $entities_permissions);
	}
  }

  if (isset($entity_type)) {
	return $entities_permissions[$entity_type];
  }
  else {
	return $entities_permissions;
  }

}

/**
 * Returns an entity_type properties that might depend on a parent entity matching property.
 * Unlike entity_toolbox_get_inherited_properties(), the array returned does not specify on which parent the property depends.
 *
 * @param null $entity_type
 *
 * @return array
 */
function entity_toolbox_get_inheritable_properties($entity_type = NULL) {
  /**
   * We cache entities_permissions since its used very often.
   */
  static $drupal_static_fast;
  if (!isset($drupal_static_fast)) {
	$drupal_static_fast['entities_inheritable_properties'] = &drupal_static(__FUNCTION__);
  }
  $entities_inheritable_properties = &$drupal_static_fast['entities_inheritable_properties'];
  if (empty($entities_inheritable_properties)) {
	if ($cache = cache_get('entities_inheritable_properties')) {
	  $entities_inheritable_properties = $cache->data;
	}
	else {
	  $toolbox_info = entity_toolbox_entity_get_info();
	  $inherited    = array();
	  foreach ($toolbox_info as $type => $type_info) {
		foreach ($type_info['properties'] as $name => $property) {
		  if ($property['type'] == 'parent') {
			foreach ($property['inherit_from_parent'] as $n => $p) {
			  if ((bool) !array_search($n, $inherited[$type])) {
				$inherited[$type][] = $n;
			  }
			}
		  }
		}
	  }
	  $entities_inheritable_properties = $inherited;
	  cache_set('entities_inheritable_properties', $entities_inheritable_properties);
	}
  }
  return isset($entity_type) ? $entities_inheritable_properties[$entity_type] : $entities_inheritable_properties;
}

/**
 * Returns an entity_type inherited properties by parent_type.
 *
 * @param null $entity_type
 * @return array
 */
function entity_toolbox_get_inherited_properties($entity_type = NULL) {
  /**
   * We cache entities_permissions since its used very often.
   */
  static $drupal_static_fast;
  if (!isset($drupal_static_fast)) {
	$drupal_static_fast['entities_inherited_properties'] = &drupal_static(__FUNCTION__);
  }
  $entities_inherited_properties = &$drupal_static_fast['entities_inherited_properties'];
  if (empty($entities_inherited_properties)) {
	if ($cache = cache_get('entities_inherited_properties')) {
	  $entities_inherited_properties = $cache->data;
	}
	else {
	  $toolbox_info = entity_toolbox_entity_get_info();
	  $inherited    = array();
	  foreach ($toolbox_info as $type => $type_info) {
		foreach ($type_info['properties'] as $name => $property) {
		  if ($property['type'] == 'parent') {
			if (!empty($property['inherit_from_parent'])) {
			  $inherited[$type][$property['reference']] = array();
			}
			foreach ($property['inherit_from_parent'] as $n => $p) {
			  $inherited[$type][$property['reference']][$n] = array(
				'parent_match' => $p['parent_match']
			  );
			}
		  }
		}
	  }
	  $entities_inherited_properties = $inherited;
	  cache_set('entities_inherited_properties', $entities_inherited_properties);
	}
  }
  return isset($entity_type) ? $entities_inherited_properties[$entity_type] : $entities_inherited_properties;

}

/**
 * Returns an entity_type properties that are parent references.
 *
 * @param null $entity_type
 *   The entity_type to get the parent properties of or NULL to return every entity_types parent properties.
 *
 * @return array
 */
function entity_toolbox_get_parent_properties($entity_type = NULL) {
  static $drupal_static_fast;
  if (!isset($drupal_static_fast)) {
	$drupal_static_fast['entities_parent_properties'] = &drupal_static(__FUNCTION__);
  }
  $entities_parent_properties = &$drupal_static_fast['entities_parent_properties'];
  if (empty($entities_parent_properties)) {
	if ($cache = cache_get('entities_parent_properties')) {
	  $entities_parent_properties = $cache->data;
	}
	else {
	  $toolbox_info      = entity_toolbox_entity_get_info();
	  $parent_properties = array();
	  foreach ($toolbox_info as $type => $type_info) {
		foreach ($type_info['properties'] as $name => $property) {
		  if ($property['type'] == 'parent') {
			$parent_properties[$type][$property['reference']] = $name;
		  }
		}
	  }
	  $entities_parent_properties = $parent_properties;
	  cache_set('entities_parent_properties', $entities_parent_properties);
	}
  }

  return isset($entity_type) ? !empty($entities_parent_properties[$entity_type]) ? $entities_parent_properties[$entity_type] : array() : $entities_parent_properties;
}

/**
 * Returns an entity_type parent entity_types.
 *
 * @param null $entity_type
 *   The entity_type to get the parent for or NULL to get every entity_type.
 *
 * @return array
 */
function entity_toolbox_get_parent_entities($entity_type = NULL) {
  static $drupal_static_fast;
  if (!isset($drupal_static_fast)) {
	$drupal_static_fast['entities_parent_entities'] = &drupal_static(__FUNCTION__);
  }
  $entities_parent_entities = &$drupal_static_fast['entities_parent_entities'];
  if (empty($entities_parent_entities)) {
	if ($cache = cache_get('entities_parent_entities')) {
	  $entities_parent_entities = $cache->data;
	}
	else {
	  $toolbox_info    = entity_toolbox_entity_get_info();
	  $parent_entities = array();
	  foreach ($toolbox_info as $type => $type_info) {
		foreach ($type_info['properties'] as $name => $property) {
		  if ($property['type'] == 'parent') {
			$parent_entities[$type][] = $property['reference'];
		  }
		}
	  }
	  $entities_parent_entities = $parent_entities;
	  cache_set('entities_parent_entities', $entities_parent_entities);
	}
  }

  return isset($entity_type) ? $entities_parent_entities[$entity_type] : $entities_parent_entities;
}

/**
 * @param $module
 */
function entity_toolbox_uninstall_module_entities($module) {
  $path = drupal_get_path('module', $module);
  require_once $path . '/' . $module . '.uninstall';
  $hook = $module . '_entities_uninstall';
  $data = $hook();
  foreach ($data as $entity_type => $info) {
	$base_table = $info['base table'];
	if (db_table_exists($base_table)) {
	  db_drop_table($base_table);
	}
  }
}

/**
 * @param $data
 * @param $router_item
 * @param $root_path
 * @param $entity_type
 * @return mixed
 */
function entity_toolbox_entity_action_link_add(&$data, $router_item, $root_path, $entity_type) {
  $toolbox_info = entity_toolbox_entity_get_info($entity_type);
  if ($toolbox_info['fieldable']) {
	if ($root_path == entity_toolbox_entity_get_path($entity_type, 'admin')) {
	  $item = menu_get_item(entity_toolbox_entity_get_path($entity_type, 'admin') . '/add');
	  if ($item['access']) {
		$data['actions']['output'][] = array(
		  '#theme' => 'menu_local_action',
		  '#link'  => $item,
		);
	  }
	}
  }
  return $data;
}

/**
 * @param null $group
 * @return array
 */
function entity_toolbox_field_category_group_get_info($group = NULL) {
  /**
   * We cache entity_toolbox_entity_info since its used very often.
   */
  static $drupal_static_fast;
  if (!isset($drupal_static_fast)) {
	$drupal_static_fast['field_category_group'] = &drupal_static(__FUNCTION__);
  }

  $field_category_group = &$drupal_static_fast['field_category_group'];
  if (empty($field_category_group)) {
	if ($cache = cache_get('field_category_group')) {
	  $field_category_group = $cache->data;
	}
	else {
	  $data = module_invoke_all('entity_toolbox_field_category_group_info');
	  drupal_alter('entity_toolbox_field_category_group_info', $data);
	  cache_set('entity_toolbox_field_category_group_info', $data);
	  $field_category_group = $data;
	}
  }

  return isset($group) ? $field_category_group[$group] : $field_category_group;
}

/**
 * @param null $group
 * @return array|null
 */
function entity_toolbox_field_category_group_get_by_entity_group($group = NULL) {
  /**
   * We cache entity_toolbox_entity_info since its used very often.
   */
  static $drupal_static_fast;
  if (!isset($drupal_static_fast)) {
	$drupal_static_fast['field_category_group_by_entity_group'] = &drupal_static(__FUNCTION__);
  }

  $field_category_group_by_entity_group = &$drupal_static_fast['field_category_group_by_entity_group'];
  if (empty($field_category_group_by_entity_group)) {
	if ($cache = cache_get('field_category_group_by_entity_group')) {
	  $field_category_group_by_entity_group = $cache->data;
	}
	else {
	  $groups        = array();
	  $entity_groups = entity_toolbox_entity_group_info();
	  foreach ($entity_groups as $entity_group => $entity_group_info) {
		$groups[$entity_group] = array();
	  }
	  $category_groups = entity_toolbox_field_category_group_get_info();
	  foreach ($category_groups as $category_group => $category_group_info) {
		$groups[$category_group_info['group']][] = $category_group;
	  }
	  $field_category_group_by_entity_group = $groups;
	  cache_set('field_category_group_by_entity_group', $field_category_group_by_entity_group);
	}
  }

  return isset($group) ? !empty($field_category_group_by_entity_group[$group]) ? $field_category_group_by_entity_group[$group] : NULL : $field_category_group_by_entity_group;
}

/**
 * @param null $category
 * @return array
 */
function entity_toolbox_field_categories_get_info($category = NULL) {
  /**
   * We cache entity_toolbox_entity_info since its used very often.
   */
  static $drupal_static_fast;
  if (!isset($drupal_static_fast)) {
	$drupal_static_fast['field_categories'] = &drupal_static(__FUNCTION__);
  }

  $field_categories = &$drupal_static_fast['field_categories'];
  if (empty($field_categories)) {
	if ($cache = cache_get('field_categories')) {
	  $field_categories = $cache->data;
	}
	else {
	  $data = module_invoke_all('entity_toolbox_field_category_info');
	  drupal_alter('entity_toolbox_field_category_info', $data);
	  cache_set('entity_toolbox_field_category_info', $data);
	  $field_categories = $data;
	}
  }

  return isset($category) ? $field_categories[$category] : $field_categories;
}

/**
 * @param null $group
 * @return array
 */
function entity_toolbox_field_categories_get_by_category_group($group = NULL) {
  /**
   * We cache entity_toolbox_entity_info since its used very often.
   */
  static $drupal_static_fast;
  if (!isset($drupal_static_fast)) {
	$drupal_static_fast['field_categories_by_category_group'] = &drupal_static(__FUNCTION__);
  }

  $field_categories_by_category_group = &$drupal_static_fast['field_categories_by_category_group'];
  if (empty($field_categories_by_category_group)) {
	if ($cache = cache_get('field_categories_by_category_group')) {
	  $field_categories_by_category_group = $cache->data;
	}
	else {
	  $field_categories_info              = entity_toolbox_field_categories_get_info();
	  $field_categories_by_category_group = array();
	  foreach ($field_categories_info as $category => $info) {
		$field_categories_by_category_group[$info['group']][$category] = $info;
	  }

	  cache_set('field_categories_by_category_group', $field_categories_by_category_group);
	}
  }

  return isset($group) ? $field_categories_by_category_group[$group] : $field_categories_by_category_group;
}

/**
 * @param null $group
 */
function entity_toolbox_field_category_get_by_entity_group($group = NULL) {
  static $drupal_static_fast;
  if (!isset($drupal_static_fast)) {
	$drupal_static_fast['field_category_by_entity_group'] = &drupal_static(__FUNCTION__);
  }

  $field_category_by_entity_group = &$drupal_static_fast['field_category_by_entity_group'];
  if (empty($field_category_by_entity_group)) {
	if ($cache = cache_get('field_category_by_entity_group')) {
	  $field_category_by_entity_group = $cache->data;
	}
	else {
	  $groups        = array();
	  $entity_groups = entity_toolbox_entity_group_info();
	  foreach ($entity_groups as $entity_group => $entity_group_info) {
		$groups[$entity_group] = array();
	  }

	}
  }
}

/**
 * @param $entity_type
 * @return null
 */
function entity_toolbox_entity_self_parent_property($entity_type) {
  $parent_properties = entity_toolbox_get_parent_properties($entity_type);
  $keys              = array_keys($parent_properties);
  $self              = array_search($entity_type, $keys);
  if ($self !== FALSE) {
	return $parent_properties[$keys[$self]];
  }
  else {
	return NULL;
  }
}

/**
 * @param $entity_type
 * @return null
 */
function entity_toolbox_entity_self_parent_property_key($entity_type) {
  $toolbox           = entity_toolbox_entity_get_info($entity_type);
  $parent_properties = entity_toolbox_get_parent_properties($entity_type);
  $keys              = array_keys($parent_properties);
  $self              = array_search($entity_type, $keys);

  if ($self !== FALSE) {
	return $toolbox['properties'][$parent_properties[$keys[$self]]]['schema field'];
  }
  else {
	return NULL;
  }
}

/**
 * @param $entity_type
 * @return mixed
 */
function entity_toolbox_entity_get_status($entity_type) {
  $query   = db_select('installed_entities', 'i')
	->fields('i', array())
	->condition('i.entity_type', $entity_type);
  $result  = $query->execute();
  $results = array();
  while ($record = $result->fetchAssoc()) {
	$results[] = $record;
  }

  return reset($results);
}

/**
 * @param $entity_type
 * @return bool
 */
function is_entity_installed($entity_type) {
  $status = entity_toolbox_entity_get_status($entity_type);
  return (bool) $status['installed'];
}

/**
 * @param $entity_type
 */
function entity_toolbox_entity_uninstall($entity_type) {
  $status            = entity_toolbox_entity_get_status($entity_type);
  $is_module_enabled = system_get_info($status['module']);
  if ($status['installed'] && !$is_module_enabled) {
	if (db_table_exists($status['table'])) {
	  db_drop_table($status['table']);
	}
  }
}

/**
 * @param $entity_type
 * @param $update_N
 */
function entity_toolbox_up($entity_type, $update_N) {

}