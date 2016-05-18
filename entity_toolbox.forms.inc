<?php

/**
 * @file
 */

/**
 * Builds an entity edit_form.
 *
 * @param $form
 * @param $form_state
 * @param $entity
 *
 * @return array
 */
function entity_toolbox_edit_form($form, &$form_state, $entity) {
  entity_toolbox_include('tree');
  /**
   * We add a wrapper so that AJAX fields can reload the form if needed.
   */
  $entity_type                    = $entity->entityType();
  $form['#prefix']                = '<div id="entity-toolbox-edit-' . $entity_type . '">';
  $form['#suffix']                = '</div>';
  $bundle                         = $entity->type;
  $toolbox_info                   = entity_toolbox_entity_get_info($entity_type, TRUE);
  $form_state[$entity_type]       = $entity;
  $form_state['entity_type']      = $entity_type;
  $form_state['#parent_entities'] = entity_parent_entities_load($entity, $entity_type);
  $form['entity_properties']      = array(
	'#title'       => $entity_type . ' properties',
	'#type'        => 'fieldset',
	'#collapsible' => TRUE,
  );
  properties_attach_form($entity_type, $entity, $form, $form_state);
//  foreach ($toolbox_info['properties'] as $name => $property) {
//	if ($property['expose']['edit_form'] === TRUE && !requires_relation_table($property)) {
//	  property_field_attach_form($form, $form_state, $entity_type, $entity, $name);
//	}
//	elseif (requires_relation_table($property)) {
//	  multi_property_field_attach_form($form, $form_state, $entity_type, $entity, $name);
//	}
//  }
  field_attach_form($entity_type, $entity, $form, $form_state);
  $bundle_field_instances = field_info_instances($entity_type, $bundle);
  $category_groups        = entity_toolbox_field_category_group_get_by_entity_group($toolbox_info['group']);
  if ($category_groups != NULL) {
	$form['categories'] = array(
	  '#type' => 'vertical_tabs',
	);
	$categories         = entity_toolbox_field_categories_get_by_category_group(reset($category_groups));
	foreach ($categories as $category => $category_info) {
	  $form['categories'][$category] = array(
		'#type'        => 'fieldset',
		'#title'       => $category_info['label'],
		'#collapsible' => TRUE,
	  );
	}
	foreach ($bundle_field_instances as $field_instance_name => $bundle_field_instance) {
	  $form['categories']['technical'][$field_instance_name] = $form[$field_instance_name];
	  unset($form[$field_instance_name]);
	}
  }
  $form['actions'] = array(
	'#type'       => 'container',
	'#attributes' => array('class' => array('form-actions')),
	'#weight'     => 400,
  );
  $submit          = array();
  if (!empty($form['#submit'])) {
	$submit += $form['#submit'];
  }
  $form['actions']['submit'] = array(
	'#type'   => 'submit',
	'#value'  => t('Save'),
	'#submit' => $submit + array('entity_toolbox_edit_form_submit'),
  );
  if (!empty($entity->name)) {
	$form['actions']['delete'] = array(
	  '#type'   => 'submit',
	  '#value'  => t('Delete'),
	  '#suffix' => l(t('Cancel'), entity_toolbox_entity_get_path($entity_type, 'admin')),
	  '#submit' => $submit + array('entity_toolbox_form_submit_delete'),
	  '#weight' => 45,
	);
  }

  $form['#validate'][] = 'entity_toolbox_edit_form_validate';

  return $form;
}

function entity_toolbox_form_entity_toolbox_edit_form_alter(&$form, &$form_state) {

}

/**
 * Builds the form to delete the entity.
 *
 * @param $form
 * @param $form_state
 * @param $entity
 *
 * @return mixed
 */
function entity_toolbox_delete_form($form, &$form_state, $entity) {
  $entity_info              = $entity->entityInfo;
  $entity_type              = $entity_info['entity_type'];
  $form_state[$entity_type] = $entity;
  $form['#submit'][]        = 'entity_toolbox_delete_form_submit';
  $form                     = confirm_form(
	$form,
	t('Are you sure you want to delete %entity_label %name?', array(
	  '%entity_label' => $entity_info['label'],
	  '%name'         => $entity->name
	)),
	entity_toolbox_entity_get_path($entity_type, 'manage'),
	'<p>' . t('This action cannot be undone.') . '</p>',
	t('Delete'),
	t('Cancel'),
	'confirm'
  );

  return $form;
}

/**
 * Form callback for form_id entity_toolbox_delete_form.
 *
 * @param $form
 * @param $form_state
 */
function entity_toolbox_delete_form_submit($form, &$form_state) {
  $entity_type = $form_state['entity_type'];
  $entity      = $form_state[$entity_type];
  $entity_info = $entity->entityInfo;

  entity_delete($entity_type, $entity);
  drupal_set_message(t('The %entity_label %name has been deleted.', array(
	'%name'         => $entity->name,
	'%entity_label' => $entity_info['label'],
  )));
  watchdog(entity_machine_name_single2plural($entity_type), 'Deleted %entity_label %name.', array(
	'%name'         => $entity->name,
	'%entity_label' => $entity_info['label'],
  ));

  $form_state['redirect'] = entity_toolbox_entity_get_path($entity_type, 'admin');
}

/**
 * Process the submit action for entity_toolbox_delete_form().
 *
 * @param $form
 * @param $form_state
 */
function entity_toolbox_form_submit_delete($form, &$form_state) {
  $entity_type            = $form_state['entity_type'];
  $entity_info            = $form_state[$entity_type]->entityInfo;
  $key_id                 = $entity_info['entity keys']['id'];
  $form_state['redirect'] = entity_toolbox_entity_get_path($entity_type, 'manage') . $form_state[$entity_type]->$key_id . '/delete';
}

/**
 * Validates the form entity_toolbox_edit_form() before submit.
 *
 * @param $form
 * @param $form_state
 */
function entity_toolbox_edit_form_validate($form, &$form_state) {
  $entity_type = $form_state['entity_type'];
  $entity      = $form_state[$entity_type];
  field_attach_form_validate($entity_type, $entity, $form, $form_state);
}

/**
 * Process the submit action for entity_toolbox_edit_form().
 *
 * @param $form
 * @param $form_state
 */
function entity_toolbox_edit_form_submit($form, &$form_state) {
  global $user;
  $entity_type  = $form_state['entity_type'];
  $entity       = entity_ui_controller($entity_type)->entityFormSubmitBuildEntity($form, $form_state);
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
  $entity->save();
  if (!empty($entity->is_new) && $entity->is_new) {
	foreach ($toolbox_info['properties'] as $name => $property) {
	  if (requires_relation_table($property)) {
		$values = $form_state['input'][$name];
		$wrapper->$name->set($values);
	  }
	}
	$entity->save();
  }
  $form_state['redirect'] = entity_toolbox_entity_get_path($entity_type, 'admin');
}

/**
 * @param        $form
 * @param        $form_state
 * @param        $entity
 * @return mixed
 */
function entity_toolbox_entity_type_edit_form($form, &$form_state, $entity, $op = 'edit') {
  entity_toolbox_include('extend');
  $entity_type = $entity->entityType();
  $entity_info = entity_toolbox_entity_get_info($entity_type);
  $bundle_of   = $entity_info['bundle of'];
  if ($op == 'clone') {
	$entity->label .= ' (cloned)';
	$entity->type = '';
  }
  $form_state['entity_type'] = $entity_type;
  $form['#prefix']           = '<div id="entity-toolbox-edit">';
  $form['#suffix']           = '</div>';
  $form['label']             = array(
	'#title'         => t('Label'),
	'#type'          => 'textfield',
	'#default_value' => $entity->label,
	'#description'   => t('The human-readable name of this ' . $entity_type . '.'),
	'#required'      => TRUE,
	'#size'          => 30,
  );
  $form['type']              = array(
	'#type'          => 'machine_name',
	'#default_value' => isset($entity->type) ? $entity->type : '',
	'#maxlength'     => 32,
	'#machine_name'  => array(
	  'exists' => $bundle_of . '_get_types',
	  'source' => array('label'),
	),
	'#description'   => t(
	  'A unique machine-readable name for this ' . $entity_type . '. It must only contain lowercase letters, numbers, and underscores.'
	),
  );
  $form['actions']           = array('#type' => 'actions');
  $form['actions']['submit'] = array(
	'#type'   => 'submit',
	'#value'  => t('Save ' . $entity_type . ' type'),
	'#weight' => 40,
  );
  $form_state[$entity_type]  = $entity;
  $form_state['entity_type'] = $entity_type;
  $form['#submit'][]         = 'entity_toolbox_type_form_submit';
  return $form;
}

/**
 * Form API submit callback for the type form.
 */
function entity_toolbox_type_form_submit(&$form, &$form_state) {
  $entity_type = $form_state['entity_type'];
  $entity      = entity_ui_controller($entity_type)->entityFormSubmitBuildEntity($form, $form_state);
  $entity->save();
  $form_state['redirect'] = entity_toolbox_entity_get_path($entity_type, 'admin');
}

/**
 * Form API submit callback for the delete button.
 */
function entity_toolbox_type_form_submit_delete(&$form, &$form_state, $entity) {
  $form_state['redirect'] = entity_toolbox_entity_get_path($entity->entityType(), 'manage') . $entity->type . '/delete';
}

/**
 * Implements hook_form_alter().
 */
function entity_toolbox_form_field_ui_field_edit_form_alter(&$form, &$form_state) {
  $entity_type   = $form['instance']['entity_type']['#value'];
  $entities_list = entity_toolbox_entity_get_list();
  $search        = array_search($entity_type, $entities_list);
  if ($search !== FALSE) {
	$toolbox_info    = entity_toolbox_entity_get_info($entity_type);
	$entity_group    = $toolbox_info['group'];
	$category_groups = entity_toolbox_field_category_group_get_info();
	foreach ($category_groups as $category_group => $category_group_info) {
	  if ($category_group_info['group'] == $entity_group) {
		$categories = entity_toolbox_field_categories_get_by_category_group($category_group);
		$options    = array();
		foreach ($categories as $field_category => $category_info) {
		  $options[$field_category] = $category_info['label'];
		}
		$form['instance']['settings']['field_category'] = array(
		  '#type'          => 'select',
		  '#title'         => t('Field category'),
		  '#empty_value'   => '',
		  '#default_value' => isset($form['#instance']['settings']['field_category']) ? $form['#instance']['settings']['field_category'] : '',
		  '#options'       => $options
		);
	  }
	}
  }
}

/**
 * Builds and attaches a form field matching a property to an entity form.
 *
 * @param $form
 *   The entity form.
 * @param $form_state
 *   The form form_state.
 * @param $entity_type
 *   The entity type.
 * @param $entity
 *   The entity.
 * @param $name
 *   The name of the property.
 *
 * @return mixed
 */
function property_field_attach_form(&$form, &$form_state, $entity_type, $entity, $name) {
  entity_toolbox_include('extend');
  $toolbox_info = entity_toolbox_entity_get_info($entity_type);
  $property     = $toolbox_info['properties'][$name];
  $id_key       = entity_toolbox_entity_get_id_key($entity_type);
  $wrapper      = entity_metadata_wrapper($entity_type, $entity);
  $is_ref       = is_property_reference($property['type']);
  if (!$is_ref) {
	$value                                                                  = !empty($entity->$name) ? $entity->$name : !empty($wrapper->$name->value()) ? $wrapper->$name->value() : property_default_value($form, $form_state, $entity_type, $entity, $name);
	$form['entity_properties'][$property['schema field']]['#default_value'] = $value;
  }

  $form['entity_properties'][$property['schema field']]['#title']       = !empty($property['field']['title']) ? t($property['field']['title']) : t($property['title']);
  $form['entity_properties'][$property['schema field']]['#required']    = $property['required']['edit_form'];
  $form['entity_properties'][$property['schema field']]['#description'] = !empty($property['description']) ? '<div class="description">' . $property['description'] . '</div>' : '';
  $form['entity_properties'][$property['schema field']]['#type']        = toolbox_property_field_widget2field_type($property['field']['widget']);

  if ($form['entity_properties'][$property['schema field']]['#type'] == 'select') {
	$form['entity_properties'][$property['schema field']]['#empty_option'] = FIELD_SELECT_DEFAULT_EMPTY_OPTION;
	if ($is_ref) {
	  $r_id_key = entity_toolbox_entity_get_id_key($property['reference']);
	  $list     = array();
	  foreach (entity_load_all($property['reference']) as $id => $loaded_entity) {
		$list[$id] = $loaded_entity->name;
	  }
	  $is_self = is_reference_self($entity_type, $property['reference']);
	  if ($is_self) {
		unset($list[entity_id($entity_type, $entity)]);
	  }
	  $form['entity_properties'][$property['schema field']]['#options']       = $list;
	  $form['entity_properties'][$property['schema field']]['#default_value'] = !empty($wrapper->$name->$r_id_key->value()) ? $wrapper->$name->$r_id_key->value() : '';
	}
	else {
	  if (!empty($property['field']['options_list'])) {
		$options = $property['field']['options_list']();
	  }
	  else {
		$options = array();
	  }
	  $form['entity_properties'][$property['schema field']]['#options'] = $options;
	}
  }
  switch ($form['entity_properties'][$property['schema field']]['#type']) {
	case 'checkbox':
	  $form['entity_properties'][$property['schema field']]['#required'] = FALSE;
	  break;
  }

  return $form;
}

/**
 * Builds and attaches a form field matching a multi value property to an entity form.
 *
 * @param $form
 *   The entity form.
 * @param $form_state
 *   The form form_state.
 * @param $entity_type
 *   The entity type.
 * @param $entity
 *   The entity.
 * @param $name
 *   The name of the property.
 *
 * @return array
 */
function multi_property_field_attach_form(&$form, &$form_state, $entity_type, $entity, $name) {
  $toolbox_info = entity_toolbox_entity_get_info($entity_type);
  $prop_info    = $toolbox_info['properties'][$name];
  $id_key       = entity_toolbox_entity_get_id_key($prop_info['reference']);
  $wrapper      = entity_metadata_wrapper($entity_type, $entity);
  $r_entities   = $wrapper->$name->value();
  $widget       = !empty($prop_info['field']['widget']) ? $prop_info['field']['widget'] : PROPERTY_REFERENCE_MULTIPLE_DEFAULT_FIELD_WIDGET;
  switch ($widget) {
	case 'autocomplete':
	  $display_marker                   = $prop_info['reference'] . '-$r-display';
	  $value_marker                     = $prop_info['reference'] . '-$r';
	  $form['entity_properties'][$name] = array(
		'#type'        => 'fieldset',
		'#title'       => t($prop_info['title']),
		'#description' => !empty($prop_info['description']) ? '<div class="description">' . $prop_info['description'] . '</div>' : ''
	  );
	  for ($r = 0; $r < count($r_entities); $r++) {
		$display_field_id                                    = preg_replace('/\$r/', $r, $display_marker);
		$field_id                                            = preg_replace('/\$r/', $r, $value_marker);
		$form['entity_properties'][$name][$display_field_id] = array(
		  '#type'          => 'textfield',
		  '#default_value' => $r_entities[$r]->name
		);
		$form['entity_properties'][$name][$field_id]         = array(
		  '#type'          => 'hidden',
		  '#default_value' => $r_entities[$r]->$id_key
		);
	  }
	  $form['entity_properties'][$name][preg_replace('/\$r/', count($r_entities), $display_marker)] = array(
		'#type'          => 'textfield',
		'#default_value' => ''
	  );
	  $form['entity_properties'][$name][preg_replace('/\$r/', count($r_entities), $value_marker)]   = array(
		'#type'          => 'hidden',
		'#default_value' => ''
	  );
	  break;
	case 'list':
	  $list     = entity_load_all($prop_info['reference']);
	  $r_list   = array();
	  $r_key_id = entity_toolbox_entity_get_id_key($prop_info['reference']);
	  foreach ($r_entities as $r_entity) {
		$r_list[] = $r_entity->$r_key_id;
	  }
	  $form['entity_properties'][$name] = array(
		'#title'       => t($prop_info['title']),
		'#description' => !empty($prop_info['description']) ? '<div class="description">' . $prop_info['description'] . '</div>' : '',
		'#type'        => 'select',
		'#multiple'    => TRUE,
	  );
	  foreach ($list as $id => $loaded_entity) {
		$list[$id] = $loaded_entity->name;
	  }
	  $is_self = is_reference_self($entity_type, $prop_info['reference']);
	  if ($is_self) {
		unset($list[entity_id($entity_type, $entity)]);
	  }
	  $form['entity_properties'][$name]['#options']       = $list;
	  $form['entity_properties'][$name]['#default_value'] = !empty($r_list) ? $r_list : array();
	  $form['entity_properties'][$name]['#empty_option']  = FIELD_SELECT_DEFAULT_EMPTY_OPTION;
	  break;
  }

  return $form;
}

function properties_attach_form($entity_type, $entity, &$form, &$form_state) {
  $toolbox_info = entity_toolbox_entity_get_info($entity_type);
  foreach ($toolbox_info['properties'] as $name => $property) {
	if ($property['expose']['edit_form'] === TRUE && !requires_relation_table($property)) {
	  property_field_attach_form($form, $form_state, $entity_type, $entity, $name);
	}
	elseif (requires_relation_table($property)) {
	  multi_property_field_attach_form($form, $form_state, $entity_type, $entity, $name);
	}
  }
  return $form;
}