<?php

/**
 * @param $data
 * @param $entity_type
 * @return mixed
 */
function entity_toolbox_entity_views_data_build(&$data, $entity_type) {
  $toolbox_info = entity_toolbox_entity_get_info($entity_type);
  $base_table   = $toolbox_info['base table'];
  $ops          = array(
	'link_' . $entity_type   => array(
	  'title'   => t('Link'),
	  'help'    => t('Provides a link to the ' . $entity_type),
	  'handler' => 'entity_toolbox_handler_link_field',
	  'type'    => $entity_type
	),
	'edit_' . $entity_type   => array(
	  'title'   => t('Edit link'),
	  'help'    => t('Provides a link to edit the ' . $entity_type),
	  'handler' => 'entity_toolbox_handler_edit_link_field',
	  'type'    => $entity_type
	),
	'delete_' . $entity_type => array(
	  'title'   => t('Delete link'),
	  'help'    => t('Provides a link to delete the ' . $entity_type),
	  'handler' => 'entity_toolbox_handler_delete_link_field',
	  'type'    => $entity_type
	),
	'operations'             => array(
	  'title'   => t('Operations links'),
	  'help'    => t('Display all operations available for this ' . $entity_type),
	  'handler' => 'entity_toolbox_handler_entity_operations_field',
	  'type'    => $entity_type
	),
  );
  foreach ($toolbox_info['properties'] as $property => $info) {
	if ($info['expose']['views'] && empty($info['relation_table'])) {
	  $data[$base_table][$info['schema field']] = array(
		'title' => t($info['title']),
		'help'  => t($info['description']),
	  );
	  if (is_property_reference($info['type']) && !requires_relation_table($info)) {
		$r_type                                                   = $info['reference'];
		$r_info                                                   = entity_get_info($r_type);
		$r_key_id                                                 = $r_info['entity keys']['id'];
		$data[$base_table][$info['schema field']]['relationship'] = array(
		  'base'       => $r_info['base table'],
		  'base field' => $r_key_id,
		  'handler'    => 'views_handler_relationship',
		  'label'      => t($info['description'] . ' (relationship)'),
		  'title'      => t($info['title']),
		  'help'       => t($info['description'] . ' (relationship)'),
		);
	  }
	  switch ($info['type']) {
		case 'id':
		  $data[$base_table][$info['schema field']]['field']    = array(
			'handler'        => 'views_handler_field_numeric',
			'click sortable' => TRUE
		  );
		  $data[$base_table][$info['schema field']]['filter']   = array('handler' => 'views_handler_filter_numeric');
		  $data[$base_table][$info['schema field']]['sort']     = array('handler' => 'views_handler_sort');
		  $data[$base_table][$info['schema field']]['argument'] = array('handler' => 'views_handler_argument_numeric');
		  break;
		case 'text_field':
		  $data[$base_table][$info['schema field']]['field']    = array(
			'handler'        => 'views_handler_field',
			'click sortable' => TRUE
		  );
		  $data[$base_table][$info['schema field']]['filter']   = array('handler' => 'views_handler_filter_string');
		  $data[$base_table][$info['schema field']]['sort']     = array('handler' => 'views_handler_sort');
		  $data[$base_table][$info['schema field']]['argument'] = array('handler' => 'views_handler_argument_string');
		  break;
		case 'text':
		  $data[$base_table][$info['schema field']]['field']    = array(
			'handler'        => 'views_handler_field',
			'click sortable' => TRUE
		  );
		  $data[$base_table][$info['schema field']]['filter']   = array('handler' => 'views_handler_filter_string');
		  $data[$base_table][$info['schema field']]['sort']     = array('handler' => 'views_handler_sort');
		  $data[$base_table][$info['schema field']]['argument'] = array('handler' => 'views_handler_argument_string');
		  break;
		case 'machine_name':
		  $data[$base_table][$info['schema field']]['field']    = array(
			'handler'        => 'views_handler_field',
			'click sortable' => TRUE
		  );
		  $data[$base_table][$info['schema field']]['filter']   = array('handler' => 'views_handler_filter_string');
		  $data[$base_table][$info['schema field']]['sort']     = array('handler' => 'views_handler_sort');
		  $data[$base_table][$info['schema field']]['argument'] = array('handler' => 'views_handler_argument_string');
		  break;
		case 'token':
		  $data[$base_table][$info['schema field']]['field']    = array(
			'handler'        => 'views_handler_field',
			'click sortable' => TRUE
		  );
		  $data[$base_table][$info['schema field']]['filter']   = array('handler' => 'views_handler_filter_string');
		  $data[$base_table][$info['schema field']]['sort']     = array('handler' => 'views_handler_sort');
		  $data[$base_table][$info['schema field']]['argument'] = array('handler' => 'views_handler_argument_string');
		  break;
		case 'serialized':
		  break;
		case 'integer':
		  $data[$base_table][$info['schema field']]['field']    = array(
			'handler'        => 'views_handler_field_numeric',
			'click sortable' => TRUE
		  );
		  $data[$base_table][$info['schema field']]['filter']   = array('handler' => 'views_handler_filter_numeric');
		  $data[$base_table][$info['schema field']]['sort']     = array('handler' => 'views_handler_sort');
		  $data[$base_table][$info['schema field']]['argument'] = array('handler' => 'views_handler_argument_numeric');
		  break;
		case 'float':
		  $data[$base_table][$info['schema field']]['field']    = array(
			'handler'        => 'views_handler_field_numeric',
			'click sortable' => TRUE
		  );
		  $data[$base_table][$info['schema field']]['filter']   = array('handler' => 'views_handler_filter_numeric');
		  $data[$base_table][$info['schema field']]['sort']     = array('handler' => 'views_handler_sort');
		  $data[$base_table][$info['schema field']]['argument'] = array('handler' => 'views_handler_argument_numeric');
		  break;
		case 'decimal':
		  $data[$base_table][$info['schema field']]['field']    = array(
			'handler'        => 'views_handler_field_numeric',
			'click sortable' => TRUE
		  );
		  $data[$base_table][$info['schema field']]['filter']   = array('handler' => 'views_handler_filter_numeric');
		  $data[$base_table][$info['schema field']]['sort']     = array('handler' => 'views_handler_sort');
		  $data[$base_table][$info['schema field']]['argument'] = array('handler' => 'views_handler_argument_numeric');
		  break;
		case 'timestamp':
		  $data[$base_table][$info['schema field']]['field']  = array(
			'handler'        => 'views_handler_field_date',
			'click sortable' => TRUE
		  );
		  $data[$base_table][$info['schema field']]['filter'] = array('handler' => 'views_handler_filter_date');
		  $data[$base_table][$info['schema field']]['sort']   = array('handler' => 'views_handler_sort_date');
		  break;
		case 'date':
		  $data[$base_table][$info['schema field']]['field']  = array(
			'handler'        => 'views_handler_field_date',
			'click sortable' => TRUE
		  );
		  $data[$base_table][$info['schema field']]['filter'] = array('handler' => 'views_handler_filter_date');
		  $data[$base_table][$info['schema field']]['sort']   = array('handler' => 'views_handler_sort_date');
		  break;
		case 'duration':
		  break;
		case 'boolean':
		  $data[$base_table][$info['schema field']]['field']  = array('handler' => 'views_handler_field_boolean');
		  $data[$base_table][$info['schema field']]['filter'] = array(
			'handler'   => 'views_handler_field_boolean_operator',
			'label'     => t($info['title']),
			'type'      => 'yes-no',
			'use equal' => TRUE,
		  );
		  $data[$base_table][$info['schema field']]['sort']   = array('handler' => 'views_handler_sort');
		  break;
		case 'parent':
		  if (!requires_relation_table($info)) {
			$data[$base_table][$info['schema field']]['field']    = array(
			  'handler'        => 'views_handler_field_numeric',
			  'click sortable' => TRUE
			);
			$data[$base_table][$info['schema field']]['filter']   = array('handler' => 'views_handler_filter_numeric');
			$data[$base_table][$info['schema field']]['sort']     = array('handler' => 'views_handler_sort');
			$data[$base_table][$info['schema field']]['argument'] = array('handler' => 'views_handler_argument_numeric');
		  }
		  break;
		case 'reference':
		  if (!requires_relation_table($info)) {
			$data[$base_table][$info['schema field']]['field']    = array(
			  'handler'        => 'views_handler_field_numeric',
			  'click sortable' => TRUE
			);
			$data[$base_table][$info['schema field']]['filter']   = array('handler' => 'views_handler_filter_numeric');
			$data[$base_table][$info['schema field']]['sort']     = array('handler' => 'views_handler_sort');
			$data[$base_table][$info['schema field']]['argument'] = array('handler' => 'views_handler_argument_numeric');
		  }
		  break;
	  }
	}
	else {
	  if ($info['expose']['views'] && is_property_reference($info['type']) && requires_relation_table($info)) {
		$class = $entity_type . '_handler_' . $property . '_field';
		if (class_exists($class)) {
		  $data[$base_table][$property] = array(
			'field' => array(
			  'title'   => t('Test multi'),
			  'help'    => t('Provides a multi val handler'),
			  'handler' => $class,
			  'type'    => 'list<' . $info['reference'] . '>',
			)
		  );
		}
	  }
	}
  }
  foreach ($ops as $op => $op_info) {
	if (class_exists($op_info['handler'])) {
	  $data[$base_table][$op]['field'] = $op_info;
	}
  }

  return $data;

}

/**
 * @param $entity_type
 * @return array
 */
function entity_toolbox_entity_admin_view_build($entity_type) {
  $toolbox_info       = entity_toolbox_entity_get_info($entity_type);
  $entity_info        = entity_get_info($entity_type);
  $base_table         = $toolbox_info['base table'];
  $plural_label_lcase = strtolower($entity_info['plural label']);
  $op_link            = 'link_' . $entity_type;

  $export            = array();
  $view              = new view();
  $view->name        = $entity_type . '_admin';
  $view->description = 'A list of all ' . $plural_label_lcase . '.';
  $view->tag         = $entity_info['plural label'];
  $view->base_table  = $base_table;
  $view->human_name  = $entity_info['plural label'];
  $view->core        = 7;
  $view->api_version = '3.0';
  $view->disabled    = FALSE;

  $handler                                              = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title']           = $toolbox_info['base table'];
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type']  = 'perm';
//  $handler->display->display_options['access']['perm']                     = 'create any ' . $entity_type . ' type';
  $handler->display->display_options['access']['perm']                     = 'access administration pages';
  $handler->display->display_options['cache']['type']                      = 'none';
  $handler->display->display_options['query']['type']                      = 'views_query';
  $handler->display->display_options['exposed_form']['type']               = 'basic';
  $handler->display->display_options['pager']['type']                      = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin']                       = 'table';
  $handler->display->display_options['empty']['area']['id']                = 'area';
  $handler->display->display_options['empty']['area']['table']             = 'views';
  $handler->display->display_options['empty']['area']['field']             = 'area';
  $handler->display->display_options['empty']['area']['label']             = 'Empty ';
  $handler->display->display_options['empty']['area']['content']           = 'No ' . $entity_info['label'] . ' have been created yet';

  foreach ($toolbox_info['properties'] as $property => $property_info) {
	if (!empty($property_info['views_display']['admin']) && $property_info['views_display']['admin']) {
	  switch ($property_info['type']) {
		case 'id':
		  $handler->display->display_options['fields'][$property_info['schema field']]['id']    = $property_info['schema field'];
		  $handler->display->display_options['fields'][$property_info['schema field']]['table'] = $base_table;
		  $handler->display->display_options['fields'][$property_info['schema field']]['field'] = $property_info['schema field'];
		  break;
		case 'text_field':
		  $handler->display->display_options['fields'][$property_info['schema field']]['id']    = $property_info['schema field'];
		  $handler->display->display_options['fields'][$property_info['schema field']]['table'] = $base_table;
		  $handler->display->display_options['fields'][$property_info['schema field']]['field'] = $property_info['schema field'];
		  break;
		case 'text':
		  $handler->display->display_options['fields'][$property_info['schema field']]['id']    = $property_info['schema field'];
		  $handler->display->display_options['fields'][$property_info['schema field']]['table'] = $base_table;
		  $handler->display->display_options['fields'][$property_info['schema field']]['field'] = $property_info['schema field'];
		  break;
		case 'machine_name':
		  $handler->display->display_options['fields'][$property_info['schema field']]['id']    = $property_info['schema field'];
		  $handler->display->display_options['fields'][$property_info['schema field']]['table'] = $base_table;
		  $handler->display->display_options['fields'][$property_info['schema field']]['field'] = $property_info['schema field'];
		  break;
		case 'token':
		  $handler->display->display_options['fields'][$property_info['schema field']]['id']    = $property_info['schema field'];
		  $handler->display->display_options['fields'][$property_info['schema field']]['table'] = $base_table;
		  $handler->display->display_options['fields'][$property_info['schema field']]['field'] = $property_info['schema field'];
		  break;
		case 'serialized':
		  break;
		case 'integer':
		  $handler->display->display_options['fields'][$property_info['schema field']]['id']    = $property_info['schema field'];
		  $handler->display->display_options['fields'][$property_info['schema field']]['table'] = $base_table;
		  $handler->display->display_options['fields'][$property_info['schema field']]['field'] = $property_info['schema field'];
		  break;
		case 'float':
		  $handler->display->display_options['fields'][$property_info['schema field']]['id']    = $property_info['schema field'];
		  $handler->display->display_options['fields'][$property_info['schema field']]['table'] = $base_table;
		  $handler->display->display_options['fields'][$property_info['schema field']]['field'] = $property_info['schema field'];
		  break;
		case 'decimal':
		  $handler->display->display_options['fields'][$property_info['schema field']]['id']    = $property_info['schema field'];
		  $handler->display->display_options['fields'][$property_info['schema field']]['table'] = $base_table;
		  $handler->display->display_options['fields'][$property_info['schema field']]['field'] = $property_info['schema field'];
		  break;
		case 'timestamp':
		  $handler->display->display_options['fields'][$property_info['schema field']]['id']                 = $property_info['schema field'];
		  $handler->display->display_options['fields'][$property_info['schema field']]['table']              = $base_table;
		  $handler->display->display_options['fields'][$property_info['schema field']]['field']              = $property_info['schema field'];
		  $handler->display->display_options['fields'][$property_info['schema field']]['date_format']        = 'short';
		  $handler->display->display_options['fields'][$property_info['schema field']]['second_date_format'] = 'long';
		  break;
		case 'date':
		  $handler->display->display_options['fields'][$property_info['schema field']]['id']    = $property_info['schema field'];
		  $handler->display->display_options['fields'][$property_info['schema field']]['table'] = $base_table;
		  $handler->display->display_options['fields'][$property_info['schema field']]['field'] = $property_info['schema field'];
		  break;
		case 'duration':
		  $handler->display->display_options['fields'][$property_info['schema field']]['id']    = $property_info['schema field'];
		  $handler->display->display_options['fields'][$property_info['schema field']]['table'] = $base_table;
		  $handler->display->display_options['fields'][$property_info['schema field']]['field'] = $property_info['schema field'];
		  break;
		case 'boolean':
		  $handler->display->display_options['fields'][$property_info['schema field']]['id']    = $property_info['schema field'];
		  $handler->display->display_options['fields'][$property_info['schema field']]['table'] = $base_table;
		  $handler->display->display_options['fields'][$property_info['schema field']]['field'] = $property_info['schema field'];
		  $handler->display->display_options['fields'][$property_info['schema field']]['type']  = 'unicode-yes-no';
		  $handler->display->display_options['fields'][$property_info['schema field']]['not']   = 0;
		  break;
		case 'parent':
		case 'reference':
		  $r_type                                                                                      = $property_info['reference'];
		  $r_info                                                                                      = entity_get_info($r_type);
		  $handler->display->display_options['relationships'][$property_info['schema field']]['id']    = $property_info['schema field'];
		  $handler->display->display_options['relationships'][$property_info['schema field']]['table'] = $entity_info['base table'];
		  if ($r_type == $entity_type) {
			$handler->display->display_options['relationships'][$property_info['schema field']]['field'] = $property_info['schema field'];
		  }
		  else {
			$handler->display->display_options['relationships'][$property_info['schema field']]['field'] = $r_info['entity keys']['id'];
		  }
		  $handler->display->display_options['fields'][$property_info['schema field']]['id']    = $property_info['schema field'];
		  $handler->display->display_options['fields'][$property_info['schema field']]['table'] = $r_info['base table'];
		  if (!empty($property_info['views_reference_property_expose']['admin'])) {
			$handler->display->display_options['fields'][$property_info['schema field']]['field'] = $property_info['views_reference_property_expose']['admin'];
		  }
		  else {
			$handler->display->display_options['fields'][$property_info['schema field']]['field'] = 'name';
		  }
		  $handler->display->display_options['fields'][$property_info['schema field']]['relationship'] = $property_info['schema field'];
		  $handler->display->display_options['fields'][$property_info['schema field']]['label']        = $property_info['description'];
		  break;
	  }
	}
  }
  $handler->display->display_options['fields'][$op_link]['id']        = $op_link;
  $handler->display->display_options['fields'][$op_link]['table']     = $base_table;
  $handler->display->display_options['fields'][$op_link]['field']     = $op_link;
  $handler->display->display_options['fields'][$op_link]['label']     = 'View';
  $handler->display->display_options['fields']['operations']['id']    = 'operations';
  $handler->display->display_options['fields']['operations']['table'] = $base_table;
  $handler->display->display_options['fields']['operations']['field'] = 'operations';


  $handler                                                         = $view->new_display('page', 'Page', $entity_type . '_admin_page');
  $handler->display->display_options['path']                       = entity_toolbox_entity_get_path($entity_type, 'admin');
  $handler->display->display_options['menu']['type']               = 'tab';
  $handler->display->display_options['menu']['menu']['name']       = 'management';
  $handler->display->display_options['menu']['title']              = $entity_info['plural label'];
  $handler->display->display_options['menu']['weight']             = '-10';
  $handler->display->display_options['tab_options']['type']        = 'tab';
  $handler->display->display_options['tab_options']['title']       = $entity_info['plural label'];
  $handler->display->display_options['tab_options']['description'] = 'Manage ' . $plural_label_lcase;
  $handler->display->display_options['tab_options']['weight']      = '0';
  $handler->display->display_options['tab_options']['name']        = 'management';
  $translatables[$entity_type . '_admin']                          = array(
	t('Master'),
	$entity_info['plural label'],
	t('more'),
	t('Apply'),
	t('Reset'),
	t('Sort by'),
	t('Asc'),
	t('Desc'),
	t('Items per page'),
	t('- All -'),
	t('Offset'),
	t('« first'),
	t('‹ previous'),
	t('next ›'),
	t('last »'),
	t('Empty '),
	t('No ' . $entity_info['label'] . ' have been created yet'),
	entity_key2field_title($entity_info['entity keys']['id']),
	t('.'),
	t(','),
	t('Label'),
	t('View'),
	t('Operations links'),
	t('Page'),
  );
  $export[$entity_type . '_admin']                                 = $view;

  return $view;
}