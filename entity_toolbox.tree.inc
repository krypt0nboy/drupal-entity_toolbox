<?php

/**
 * Returns an array of the loaded parent entities of an entity.
 *
 * @param $entity
 *   The entity for which the parents should be loaded.
 *
 * @param $entity_type
 * @param $parents_type
 *
 * @return array
 */
function entity_parent_entities_load($entity, $entity_type, $parents_type = NULL) {
  $wrapper           = entity_metadata_wrapper($entity_type, $entity);
  $toolbox_info      = entity_toolbox_entity_get_info($entity_type, TRUE);
  $parent_properties = entity_toolbox_get_parent_properties($entity_type);
  $parents           = array();

  foreach ($parent_properties as $parent_property) {
	$p_type           = $toolbox_info['properties'][$parent_property]['reference'];
	$parents[$p_type] = $wrapper->$parent_property->value();
  }

  return $parents;
}

/**
 * Returns the entities that have the provided entity as possible parent.
 *
 * @param null $entity_type
 *   The entity_type or NULL for every entity_type.
 * @return array
 */
function entity_toolbox_entity_children_get_info($entity_type = NULL) {
  /**
   * We cache entities_children_info since its used very often.
   */
  static $drupal_static_fast;
  if (!isset($drupal_static_fast)) {
	$drupal_static_fast['entities_children_info'] = &drupal_static(__FUNCTION__);
  }
  $entities_children_info = &$drupal_static_fast['entities_children_info'];
  if (empty($entities_children_info)) {
	if ($cache = cache_get('entities_children_info')) {
	  $entities_children_info = $cache->data;
	}
	else {
	  $toolbox_info = entity_toolbox_entity_get_info(NULL, TRUE);
	  $infos        = array();
	  foreach ($toolbox_info as $type => $info) {
		foreach ($info['properties'] as $name => $property) {
		  if ($property['type'] == 'parent') {
			$infos[$property['reference']][] = $type;
		  }
		}
	  }
	  $entities_children_info = $infos;
	  cache_set('entities_children_info', $entities_children_info);
	}
  }

  if (isset($entity_type)) {
	return $entities_children_info[$entity_type];
  }
  else {
	return $entities_children_info;
  }
}

/**
 * Returns the children entities of an entity.
 *
 * @param      $entity
 * @param null $child_type
 *   The machine_name of the children entity_type or NULL to return an array of all the children types.
 *
 * @return array
 */
function entity_children_entities_load($entity, $child_type = NULL) {
  $entity_type       = $entity->entityType();
  $children_entities = array();
  $children_info     = entity_toolbox_entity_children_get_info($entity_type);

  foreach ($children_info as $child_type) {
	$child_info    = entity_get_info($child_type);
	$id_key        = $child_info['entity keys']['id'];
	$reference_key = $child_info['entity keys'][$entity_type];
	$query         = new EntityFieldQuery();
	$query
	  ->entityCondition('entity_type', $child_type, '=')
	  ->propertyCondition($reference_key, $entity_type);
	$result = $query->execute();
	if (isset($result[$child_type])) {
	  foreach ($result[$child_type] as $record) {
		$loaded_entity                                    = entity_load($child_type, array($record->$id_key));
		$children_entities[$child_type][$record->$id_key] = reset($loaded_entity);
	  }
	}
  }

  return isset($child_type) ? $children_entities[$child_type] : $children_entities;
}

/**
 * Returns an array containing the entity_types and entities of N level in a group.
 *
 * @param      $group
 *   The group machine_name for which to build the tree.
 * @see hook_entity_toolbox_group_info().
 * @see hook_entity_toolbox_group_attach_info().
 *
 * @param null $depth
 *   The depth level to build the tree or NULL for infinite level.
 * @param bool $entity_load
 *   A boolean set to TRUE to include the loaded entities in the tree or FALSE to include ids alone.
 *
 * @return array
 */
function entity_toolbox_entity_group_build_tree($group, $depth = NULL, $entity_load = TRUE) {
  //get group entities
  //get top level entity : load all
  //get sub level entities : load all
}

/**
 * @param $entity_type
 * @param $parent_type
 * @return array
 */
function entity_toolbox_family_tree_build($entity_type, $parent_type) {
  $children  = &drupal_static(__FUNCTION__, array());
  $parents   = &drupal_static(__FUNCTION__ . ':parents', array());
  $toolbox   = entity_toolbox_entity_get_info($entity_type);
  $p_toolbox = ($entity_type == $parent_type) ? $toolbox : entity_toolbox_entity_get_info($parent_type);
  $parent    = 0;

  if (empty($children)) {
	$process_parents   = array();
	$process_parents[] = $parent;
	$children          = array();
	$parents           = array();
	$tree              = array();
	$query             = db_select($toolbox['base table'], 'e')
	  ->fields('e')
	  ->orderBy('e.parent_product_family_id')
	  ->orderBy('e.product_family_id');
	$result            = $query->execute();
	foreach ($result as $item) {
	  $children[$item->parent_product_family_id][] = $item->product_family_id;
	  $parents[$item->product_family_id][]         = $item->parent_product_family_id;
	}
	while (count($process_parents)) {
	  $parent       = array_pop($process_parents);
	  $depth        = count($process_parents);
	  $has_children = FALSE;
	  $child        = current($children[$parent]);
	}
  }


  return $tree;
}