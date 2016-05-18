<?php

/**
 * @file
 *
 * This file contains functions which purpose is to extend/simplify the entity API module use.
 */

/**
 * Returns the type of an entity.
 *
 * @param $entity
 *  The entity to get the type from.
 *
 * @return string|null
 *  Returns NULL if method 'entityType' does not exist for the entity.
 */
function entity_type($entity) {
  return method_exists($entity, 'entityType') ? $entity->entityType() : NULL;
}

/**
 * Returns the bundle of an entity.
 *
 * @param $entity
 *  The entity to query.
 *
 * @TODO : add exceptions for entities that do not support entity::entityType();
 *
 * @return string
 */
function entity_bundle($entity) {
  return method_exists($entity, 'entityType') ? $entity->entityType() : NULL;
}

/**
 * Returns a list of entities managed by a module.
 *
 * @param null $module
 *   The name of the module or NULL for every module.
 *
 * @return array
 */
function entity_toolbox_module_get_entities($module = NULL) {
  static $drupal_static_fast;
  if (!isset($drupal_static_fast)) {
	$drupal_static_fast['modules_entities'] = &drupal_static(__FUNCTION__);
  }

  $modules_entities = &$drupal_static_fast['modules_entities'];
  if (empty($modules_entities)) {
	if ($cache = cache_get('modules_entities')) {
	  $modules_entities = $cache->data;
	}
	else {
	  $entities_info = entity_get_info();
	  $list          = array();
	  foreach ($entities_info as $entity_type => $info) {
		if (isset($info['module'])) {
		  $list[$info['module']][] = $entity_type;
		}
	  }
	  $modules_entities = $list;
	  if (!empty($modules_entities)) {
		cache_set('modules_entities', $list);
	  }
	}
  }

  return isset($module) ? $modules_entities[$module] : $modules_entities;
}

/**
 * Returns the bundles info of an entity_type.
 *
 * @param      $entity_type
 *   The entity_type for which to get the bundles.
 * @param null $type
 *   The bundle for which to get the info or NULL for all the bundles.
 *
 * @return array|mixed
 */
function entity_get_types($entity_type, $type = NULL) {
  $toolbox_info  = entity_toolbox_entity_get_info($entity_type);
  $bundle_entity = $toolbox_info['bundle_entity'];
  $types         = entity_load_multiple_by_name($bundle_entity, isset($type) ? array($type) : FALSE);

  return isset($type) ? reset($types) : $types;
}

/**
 * @param      $entity_type
 * @param null $type
 * @return array
 */
function entity_load_all($entity_type, $type = NULL) {
  $entities    = array();
  $entity_info = entity_get_info($entity_type);
  $id_key      = $entity_info['entity keys']['id'];

  $query = new EntityFieldQuery();
  $query
	->entityCondition('entity_type', $entity_type, '=');
  if (isset($type)) {
	$query->propertyCondition('type', $type);
  }
  $result = $query->execute();
  if (isset($result[$entity_type])) {
	foreach ($result[$entity_type] as $record) {
	  $loaded_entity              = entity_load($entity_type, array($record->$id_key));
	  $entities[$record->$id_key] = reset($loaded_entity);
	}
  }
  return $entities;
}

function entity_load_orphans($entity_type, $parent_type) {
  $entities     = array();
  $entity_info  = entity_get_info($entity_type);
  $toolbox      = entity_toolbox_entity_get_info($entity_type);
  $parent_types = entity_toolbox_get_parent_properties($entity_type);
  $parent_key   = $toolbox['properties'][$parent_types[$parent_type]]['schema field'];
  $id_key       = $entity_info['entity keys']['id'];

  $query = new EntityFieldQuery();
  $query
	->entityCondition('entity_type', $entity_type, '=')
	->propertyCondition($parent_key, 0, '=');
  $result = $query->execute();
  if (isset($result[$entity_type])) {
	foreach ($result[$entity_type] as $record) {
	  $loaded_entity              = entity_load($entity_type, array($record->$id_key));
	  $entities[$record->$id_key] = reset($loaded_entity);
	}
  }
  return $entities;
}

function entity_load_matching_parent($parent_id, $parent_type, $entity_type) {
  $entities     = array();
  $entity_info  = entity_get_info($entity_type);
  $toolbox      = entity_toolbox_entity_get_info($entity_type);
  $parent_types = entity_toolbox_get_parent_properties($entity_type);
  $parent_key   = $toolbox['properties'][$parent_types[$parent_type]]['schema field'];
  $id_key       = $entity_info['entity keys']['id'];

  $query = new EntityFieldQuery();
  $query
	->entityCondition('entity_type', $entity_type, '=')
	->propertyCondition($parent_key, $parent_id, '=');
  $result = $query->execute();
  if (isset($result[$entity_type])) {
	foreach ($result[$entity_type] as $record) {
	  $loaded_entity              = entity_load($entity_type, array($record->$id_key));
	  $entities[$record->$id_key] = reset($loaded_entity);
	}
  }
  return $entities;
}