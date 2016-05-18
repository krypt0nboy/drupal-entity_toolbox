<?php

/**
 * @file
 *
 */

/**
 * Implements hook_entity_insert().
 */
function entity_toolbox_entity_insert($entity, $type) {

}

/**
 * Implements hook_entity_update().
 */
function entity_toolbox_entity_update($entity, $type) {
  $bundle       = $entity->type;
  $toolbox_info = entity_toolbox_entity_get_info($type, TRUE);
}

/**
 * Implements hook_entity_delete().
 *
 * @TODO : On delete, confirm if children_entities should be deleted too.
 */
function entity_toolbox_entity_delete($entity, $type) {

}

/**
 * Returns an array for drupal_set_breadcrumb().
 *
 * @param string $path
 *   The entity path.
 *
 * @return array
 */
function entity_toolbox_build_breadcrumbs($path) {
  $split            = explode('/', $path);
  $breadcrumb       = array();
  $breadcrumb_paths = array();

  for ($i = 0; $i < count($split); $i++) {
	$breadcrumb_paths[] = $split[$i];
	$bcp                = '';
	for ($ii = 0; $ii <= $i; $ii++) {
	  $bcp .= $breadcrumb_paths[$ii] . '/';
	}
	$breadcrumb[] = l(t(ucfirst($split[$i])), $bcp);
  }
  return $breadcrumb;
}

/**
 * Updates every child entity inheritable properties.
 *
 * @param       $entity
 *  The parent entity.
 * @param array $children_entities
 *  An array of children entities or an empty array to get the children in the function.
 *
 */
function entity_update_children_entities($entity, array $children_entities = array()) {
  entity_toolbox_include('tree');
  foreach (empty($children_entities) ? entity_children_entities_load($entity) : $children_entities as $child_entity) {
	entity_update_from_parent_entity($child_entity, $entity);
  }
}

/**
 * @param $entity
 * @param $parent_entity
 */
function entity_update_from_parent_entity($entity, $parent_entity) {
  entity_toolbox_include('tree');
  entity_toolbox_include('extend');

  $toolbox_info         = entity_toolbox_entity_get_info(entity_type($entity));
  $toolbox_parent_info  = entity_toolbox_entity_get_info(entity_type($parent_entity));
  $inherited_properties = entity_toolbox_get_inherited_properties(entity_type($entity));

  foreach ($inherited_properties[entity_type($parent_entity)] as $property => $inherited_property) {
	$entity->$property = $parent_entity->$inherited_property['parent_property'];
  }
}

/**
 * @param $entity_type
 * @return array
 */
function entity_UI_controller_menu_build($entity_type) {
  entity_toolbox_include('extend');
  $entity_info  = entity_get_info($entity_type);
  $toolbox_info = entity_toolbox_entity_get_info($entity_type);
  $path         = entity_toolbox_entity_get_path($entity_type, 'admin');
  $items        = array();
  $id_count     = count(explode('/', $path));
  $label        = entity_type2entity_label($entity_type);
  $label_plural = entity_type2entity_label_plural($entity_type);

  if ($toolbox_info['fieldable']) {
	$wildcard              = $entity_info['admin ui']['menu wildcard'];
	$create_callback       = $entity_type . '_create';
	$items[$path]          = array(
	  'title'            => $label_plural,
	  'type'             => MENU_LOCAL_TASK,
	  'description'      => 'Add edit and update ' . $label_plural . '.',
	  'page callback'    => 'system_admin_menu_block_page',
	  'access arguments' => array('access administration pages'),
	  'file path'        => drupal_get_path('module', 'system'),
	  'file'             => 'system.admin.inc',
	);
	$items[$path . '/add'] = array(
	  'title'            => 'Add a ' . $label,
	  'description'      => 'Add a new ' . $label,
	  'page callback'    => 'entity_add_page',
	  'page arguments'   => array($entity_type),
	  'access callback'  => $entity_type . '_access',
	  'access arguments' => array('edit'),
	  'type'             => MENU_NORMAL_ITEM,
	  'weight'           => 20,
	  'file'             => 'entity_toolbox.entities.inc',
	  'file path'        => ENTITY_TOOLBOX_PATH,
	);
	foreach (entity_get_types($entity_type) as $type) {
	  $items[$path . '/add/' . $type->type] = array(
		'title'            => 'Add ' . $type->label,
		'page callback'    => 'entity_form_wrapper',
		'page arguments'   => array($create_callback(array('type' => $type->type))),
		'access callback'  => $entity_type . '_access',
		'access arguments' => array('edit', 'edit ' . $type->type),
		'file'             => 'entity_toolbox.entities.inc',
		'file path'        => ENTITY_TOOLBOX_PATH,
	  );
	}
	$items[$path . '/' . $entity_type . '/' . $wildcard]             = array(
	  'page callback'    => 'entity_form_wrapper',
	  'page arguments'   => array($id_count + 1),
	  'access callback'  => $entity_type . '_access',
	  'access arguments' => array('edit', $id_count + 1),
	  'weight'           => 0,
	  'context'          => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
	  'file'             => 'entity_toolbox.entities.inc',
	  'file path'        => ENTITY_TOOLBOX_PATH,
	);
	$items[$path . '/' . $entity_type . '/' . $wildcard . '/edit']   = array(
	  'title'   => 'Edit',
	  'type'    => MENU_DEFAULT_LOCAL_TASK,
	  'weight'  => -10,
	  'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
	);
	$items[$path . '/' . $entity_type . '/' . $wildcard . '/view']   = array(
	  'title'            => 'View',
	  'weight'           => -10,
	  'page callback'    => $entity_type . '_page_view',
	  'page arguments'   => array($id_count + 1),
	  'access callback'  => $entity_type . '_access',
	  'access arguments' => array('view', $id_count + 1),
	  'type'             => MENU_LOCAL_TASK,
	);
	$items[$path . '/' . $entity_type . '/' . $wildcard . '/delete'] = array(
	  'title'            => 'Delete',
	  'page callback'    => 'entity_delete_form_wrapper',
	  'page arguments'   => array($id_count + 1),
	  'access callback'  => $entity_type . '_access',
	  'access arguments' => array('edit', $id_count + 1),
	  'type'             => MENU_LOCAL_TASK,
	  'context'          => MENU_CONTEXT_INLINE,
	  'weight'           => 10,
	  'file'             => 'entity_toolbox.entities.inc',
	  'file path'        => ENTITY_TOOLBOX_PATH,
	);
	$items[$entity_type . '/' . $wildcard]                           = array(
	  'title callback'   => $entity_type . '_page_title',
	  'title arguments'  => array(1),
	  'page callback'    => $entity_type . '_page_view',
	  'page arguments'   => array(1),
	  'access callback'  => $entity_type . '_access',
	  'access arguments' => array('view', 1),
	  'type'             => MENU_CALLBACK,
	);
  }
  else {
	$items[$path]                                       = array(
	  'title'            => $label_plural,
	  'page callback'    => 'drupal_get_form',
	  'page arguments'   => array(
		$entity_type . '_overview_form',
		$entity_type
	  ),
	  'description'      => 'Manage ' . $label_plural . ', including adding
		and removing fields and the display of fields.',
	  'access callback'  => 'entity_access',
	  'access arguments' => array('view', $entity_type),
	  'file'             => 'includes/entity.ui.inc',
	);
	$items[$path . '/add']                              = array(
	  'title'            => 'Add a ' . $label,
	  'page callback'    => 'entity_type_form_wrapper',
	  'page arguments'   => array(entity_create($entity_type, array())),
	  'access callback'  => 'entity_access',
	  'access arguments' => array('create', $entity_type),
	  'type'             => MENU_LOCAL_ACTION,
	  'weight'           => 20,
	  'file'             => 'entity_toolbox.entities.inc',
	  'file path'        => ENTITY_TOOLBOX_PATH,
	);
	$items[$path . '/manage/%entity_object']            = array(
	  'title'            => 'Edit',
	  'title callback'   => 'entity_label',
	  'title arguments'  => array($entity_type, $id_count + 1),
	  'page callback'    => 'entity_type_form_wrapper',
	  'page arguments'   => array($id_count + 1, $entity_type),
	  'load arguments'   => array($entity_type),
	  'access callback'  => 'entity_access',
	  'access arguments' => array('update', $entity_type, $id_count + 1),
	  'file'             => 'entity_toolbox.entities.inc',
	  'file path'        => ENTITY_TOOLBOX_PATH,
	);
	$items[$path . '/manage/%entity_object/edit']       = array(
	  'title'          => 'Edit',
	  'load arguments' => array($entity_type),
	  'type'           => MENU_DEFAULT_LOCAL_TASK,
	);
	$items[$path . '/manage/%entity_object' . '/clone'] = array(
	  'title'            => 'Clone',
	  'page callback'    => 'entity_type_form_wrapper',
	  'page arguments'   => array($id_count + 1, $entity_type),
	  'load arguments'   => array($entity_type),
	  'access callback'  => 'entity_access',
	  'access arguments' => array('create', $entity_type),
	);
	$items[$path . '/manage/%entity_object' . '/%']     = array(
	  'page callback'    => 'drupal_get_form',
	  'page arguments'   => array(
		$entity_type . '_operation_form',
		$entity_type,
		$id_count + 1,
		$id_count + 2
	  ),
	  'load arguments'   => array($entity_type),
	  'access callback'  => 'entity_access',
	  'access arguments' => array('delete', $entity_type, $id_count + 1),
	  'file'             => 'includes/entity.ui.inc',
	);

	if (!empty($entity_info['exportable'])) {
	  $items[$path . '/import'] = array(
		'title callback'   => 'entity_ui_get_action_title',
		'title arguments'  => array('import', $entity_type),
		'page callback'    => 'drupal_get_form',
		'page arguments'   => array(
		  $entity_type . '_operation_form',
		  $entity_type,
		  NULL,
		  'import'
		),
		'access callback'  => 'entity_access',
		'access arguments' => array('create', $entity_type),
		'file'             => 'includes/entity.ui.inc',
		'type'             => MENU_LOCAL_ACTION,
	  );
	}

  }

  return $items;
}

/**
 * Sets the edit breadcrumb and returns the edit/create form for an entity.
 *
 * @param $entity
 *   The entity to be edited/created.
 *
 * @return array
 */
function entity_form_wrapper($entity) {
  entity_set_breadcrumb($entity->entityType(), 'admin');
  return drupal_get_form('entity_toolbox_edit_form', $entity);
}

/**
 * Returns the delete form for an entity.
 *
 * @param $entity
 *   The entity to delete.
 *
 * @return array
 */
function entity_delete_form_wrapper($entity) {
  return drupal_get_form('entity_toolbox_delete_form', $entity);
}

/**
 * Returns the generic path for an entity action.
 *
 * @param $entity_type
 *   The entity_type to get the path from.
 * @param $alias
 *   The alias of the path, which can be :
 *    - root
 *    - admin
 *    - manage
 */
function entity_set_breadcrumb($entity_type, $alias) {
  $path = entity_toolbox_entity_get_path($entity_type, $alias);
  drupal_set_breadcrumb(entity_toolbox_build_breadcrumbs($path));
}

/**
 * @param $entity_type
 * @return mixed
 */
function entity_add_page($entity_type) {

  $item    = menu_get_item();
  $content = system_admin_menu_block($item);

  if (count($content) == 1) {
	$item = array_shift($content);
	drupal_goto($item['href']);
  }

  return theme('entity_add_list', array(
	'type'    => $entity_type,
	'content' => $content,
  ));
}

function entity_type_form_wrapper($entity) {
  entity_set_breadcrumb($entity->entityType(), 'admin');
  return drupal_get_form('entity_toolbox_entity_type_edit_form', $entity);
}

/**
 * Returns an array for a multiple references property, whose entity_type has a relation table.
 *
 * @param $entity_type
 *   The type of the entity.
 * @param $entity
 *   The entity to get the values for.
 * @param $property
 *   The property name.
 *
 * @return array
 */
function entity_get_multiple_values_property($entity_type, $entity, $property) {
  $toolbox_info = entity_toolbox_entity_get_info($entity_type);
  $prop         = $toolbox_info['properties'][$property];
  $key_id       = $toolbox_info['id_property'];
  $r_type       = $prop['reference'];
  $r_info       = entity_toolbox_entity_get_info($r_type);
  $r_info       = isset($r_info) ? $r_info : entity_get_info($r_type);
  $r_key_id     = isset($r_info['id_property']) ? $r_info['id_property'] : $r_info['entity keys']['id'];
  $r_table      = $prop['relation_table']['table'];
  $query        = db_select($r_table, 'e')
	->fields('e', array())
	->condition('e.' . $key_id, $entity->$key_id)
	->execute();
  $results      = array();
  while ($result = $query->fetchAssoc()) {
	$results[] = $result[$r_key_id];
  }
  return $results;
}

/**
 * Inserts/Deletes relations in an entity relation table.
 *
 * @param $entity_type
 *   The entity type. Has to be registered through entity toolbox.
 * @param $entity
 *   The entity.
 * @param $name
 *   The property name. Has to be a multi value reference.
 * @param $values
 *   The values passed by the setter.
 * @throws \Exception
 */
function entity_set_multiple_values_property($entity_type, $entity, $name, $values) {
  $values           = gettype($values) == 'array' ? $values : array();
  $wrapper          = entity_metadata_wrapper($entity_type, $entity);
  $toolbox_info     = entity_toolbox_entity_get_info($entity_type);
  $entity_id        = entity_id($entity_type, $entity);
  $property         = $toolbox_info['properties'][$name];
  $r_type           = $property['reference'];
  $r_id_key         = entity_toolbox_entity_get_id_key($r_type);
  $rel_table        = $property['relation_table']['table'];
  $rel_self_key     = $property['relation_table']['keys']['self'];
  $rel_ref_key      = $property['relation_table']['keys']['reference'];
  $current_values   = $wrapper->$name->value();
  $current_val_list = array();
  $del_ids          = array();
  $add_ids          = array();
  foreach ($current_values as $cur) {
	$current_val_list[] = $cur->$r_id_key;
	if (array_search($cur->$r_id_key, $values) === FALSE) {
	  $del_ids[] = $cur->$r_id_key;
	}
  }
  foreach ($values as $value) {
	if (array_search($value, $current_val_list) === FALSE) {
	  $add_ids[] = $value;
	}
  }
  foreach ($add_ids as $add_id) {
	$query = db_insert($rel_table)
	  ->fields(
		array(
		  $rel_self_key => $entity_id,
		  $rel_ref_key  => $add_id
		)
	  )
	  ->execute();
  }
  if (!empty($del_ids)) {
	db_delete($rel_table)
	  ->condition($rel_self_key, $entity_id)
	  ->condition($rel_ref_key, $del_ids, 'IN')
	  ->execute();
  }
}

/**
 * Default parent/reference entity attribute getter.
 * @param bool  $entity
 * @param array $options
 * @param       $name
 * @param       $type
 * @return array
 */
function entity_get_attributes($entity = FALSE, array $options, $name, $type) {
  $toolbox_info = entity_toolbox_entity_get_info($type);
  $properties   = $toolbox_info['properties'];
  $property     = $properties[$name];
  if (!empty($property)) {
	if (is_property_reference($property['type'])) {
	  if (!is_reference_multiple($property)) {
		return $entity->$property['schema field'];
	  }
	  else {
		return entity_get_multiple_values_property($type, $entity, $name);
	  }
	}
  }
}

/**
 * Default parent/reference entity attribute setter.
 * @TODO : manage languages
 * @param $entity
 * @param $name
 * @param $value
 * @param $lang
 * @param $type
 */
function entity_set_attributes($entity, $name, $value, $lang, $type) {
  $toolbox_info = entity_toolbox_entity_get_info($type);
  $properties   = $toolbox_info['properties'];
  $property     = $properties[$name];
  if (!empty($property)) {
	if (is_property_reference($property['type'])) {
	  if (!is_reference_multiple($property)) {
		$entity->$property['schema field'] = $value;
	  }
	  else {
		entity_set_multiple_values_property($type, $entity, $name, $value);
	  }
	}
  }
}

if (class_exists('EntityInlineEntityFormController')) {
  /**
   * The Controller for entities declared through entity_toolbox for Inline Entity Form.
   *
   * Class EntityToolboxEntityInlineEntityFormController
   */
  class EntityToolboxEntityInlineEntityFormController extends EntityInlineEntityFormController {
	public function entityForm($entity_form, &$form_state) {
	  entity_toolbox_include('extend');
	  $entity                    = $entity_form['#entity'];
	  $entity_type               = entity_type($entity);
	  $form_state['entity_type'] = $entity_type;
	  $extra_fields              = field_info_extra_fields($entity_type, $entity->type, 'form');
	  $entity_form['entity_properties'] = array(
		'#title'       => $entity_type . ' properties',
		'#type'        => 'fieldset',
		'#collapsible' => TRUE,
	  );
	  properties_attach_form($entity_type, $entity, $entity_form, $form_state);
//	  field_attach_form($entity_type, $entity, $entity_form, $form_state);
	  return $entity_form;
	}

	public function entityFormSubmit(&$entity_form, &$form_state) {
	  global $user;
	  $entity_type  = $this->entityType;
	  $entity       = $entity_form['#entity'];
	  $toolbox_info = entity_toolbox_entity_get_info($entity_type);
	  $wrapper      = entity_metadata_wrapper($entity_type, $entity);
	  if ($entity->is_new = isset($entity->is_new) ? $entity->is_new : 0) {
		$entity->created = REQUEST_TIME;
		$entity->uid     = $user->uid;
	  }
	  /**
	   * If the entity is new, we save multi value properties after the entity has been inserted and assigned an entity ID.
	   */
	  if (!$entity->is_new) {
		foreach ($toolbox_info['properties'] as $name => $property) {
		  if (requires_relation_table($property)) {
			$values = $form_state['input'][$name];
			$wrapper->$name->set($values);
		  }
		}
	  }
	  $entity->changed = REQUEST_TIME;
//	  $entity->save();
	  if (!empty($entity->is_new) && $entity->is_new) {
		foreach ($toolbox_info['properties'] as $name => $property) {
		  if (requires_relation_table($property)) {
			$values = $form_state['input'][$name];
			$wrapper->$name->set($values);
		  }
		}
//		$entity->save();
	  }
	}
  }
}