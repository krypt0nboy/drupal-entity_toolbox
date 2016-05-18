<?php

/**
 * @file
 * This file contains the instructions to use entity_toolbox hooks.
 */

/**
 * @TODO : add hook_entity_toolbox_action_template_info().
 * @TODO : add hook_entity_toolbox_property_template_info().
 * @TODO : add hook_entity_toolbox_menu_template_info().
 * @TODO : add hook_entity_toolbox_view_template_info().
 */

/**
 * Returns an array containing info to declare an entity_type.
 * The data will be used for hook_entity_info() among other hooks. Foreach fieldable entity type, should be declared a non fieldable entity_type.
 *
 *  - entity_type (string) : the entity_type machine name.
 *  - base_table (string) : the base table that store the entity_type entities.
 *    Defaults to 'entity_type'.
 *  - fieldable (boolean) : is the entity fieldable ?
 *    Defaults to FALSE.
 *  - bundle_entity (string) (optional) : the bundle entity_type (Eg : foo -> foo_type).
 *    Defaults to the entity machine name + '_type' and NULL if the entity is not fieldable.
 *  - module (string) : the module managing the entities.
 *  - paths (array) :
 *    - root (string) : the root path for the entity_type urls.
 *    - admin (string) : the admin path to list the entity_type entities.
 *    - manage (string) :
 *  - has_revisions (boolean) : if the entity has revisions enabled.
 *    Note that once the entity installed, this update has to be made through hook_entity_toolbox_entity_info_update().
 *    Defaults to FALSE.
 *  - translatable (boolean) : if the entity is translatable.
 *    Note that once the entity installed, this update has to be made through hook_entity_toolbox_entity_info_update().
 *    Defaults to FALSE.
 *  - properties (array) :
 *    - property_name (string) =>
 *     - key (string) : the property key.
 *     - is_key (boolean) : should the property be processed as key ?
 *       Defaults to FALSE.
 *     - title (string) : the title of the property (used to display the property form field).
 *       Defaults to machine_name2human_name('property_name').
 *     - description (string) : a short description of the property.
 *     - help (string) : a more detailed description of the property, and how it should be filled.
 *     - is_parent (boolean) : is the property a parent property. If true, the value "type" will be set to "reference".
 *       Defaults to FALSE.
 *     - inherit_from_parent (array) : an array containing arguments for a parent inheritable property.
 *        - property_name (array) : an array containing the argument for an inheritable property, which the key is the current entity_type property name.
 *         - parent_property (string) : the matching parent property name.
 *         - can_override (boolean) : can the heritage be overriden ?
 *         - override_conditions (array) : an array containing the conditions for the property to be overriden.
 *     - type (string) : the type of data for that property.
 *       Available types are :
 *        id - The entity id.
 *        text_field - Any type of string below 255 length.
 *        text_small - Any type of string above 255 and below 16MB.
 *        text_normal - Any type of text above 255 and below 16MB.
 *        text_large - Any type of text from 16MB.
 *        token - A text that only accepts lower case and no special char.
 *        machine_name - An explicit machine name type field. Lowercase and no special chars.
 *        serialized - A serialized array.
 *        serialized_big - A serialized array.
 *        integer - A regular integer php value.
 *        float - A regular float php value.
 *        decimal - A regular decimal php value.
 *        timestamp - A UNIX timestamp.
 *        date - A date.
 *        duration - A duration in seconds.
 *        boolean - A regular boolean value.
 *        reference - An entity.
 *        parent - A parent entity.
 *     - source (string) : the property table name.
 *       Defaults to 'base_table'.
 *     - multiple (boolean) : is the property multi value ?
 *       Defaults to FALSE.
 *     - required (boolean) : is the property required to insert the entity ?
 *       Defaults to FALSE.
 *     - schema (array) : an array containing the schema field data.
 *     - relation_table (array) : an array containing the relation_table data.
 *       If the property is of type 'reference' and is 'multiple', a relation table will be created.
 *        - table : the name of the relation table.
 *          Defaults to 'entity_type' + _has_ + the reference entity 'entity_type'.
 *          (Eg : For product that have a multiple sub_products property : 'product_has_sub_products').
 *        - descriptions : a description of the relation table.
 *     - callbacks (array) (optional) : an array containing the property callbacks.
 *      - getter (string) : the getter callback.
 *      - setter (string) : the setter callback.
 *       Defaults to NULL.
 *     - permissions (array) (optional) : an array containing the setter/getter permissions.
 *     - field (array) : an array containing the entity edit_form field widget.
 *      - widget (string) :
 *        Available widgets are :
 *         textfield : a basic textfield form field.
 *         list : a select list. Can contain arbitrary values or entities.
 *         autocomplete : a textfield with an autocomplete callback.
 *         checkbox : a checkbox.
 *         textarea : a textarea.
 *         file : A file upload field.
 *        Defaults to textfield.
 *      - default_value (mixed) (optional) : the default value for the field.
 *      - max_length (int) :
 *      - weight (int) : the weight of the property field in a given form.
 *  - id_property (string) : the name of the key property.
 *    Defaults to the entity machine name + '_id' if this property exists when the entity is fieldable and
 *    to 'id' when the entity is not fieldable.
 *  - bundles (array) :
 *  - group (string) : Attaches this entity to a group of entities.
 *    (Eg : The entity product could belong to the group 'catalog'.).
 *  - field_category_group (string) : Adds an option to associate an entity field to a field category.
 *
 * @return array
 *
 * @see hook_entity_toolbox_entity_info_alter().
 * @see hook_entity_toolbox_ENTITY_TYPE_properties_update_N().
 * @see hook_entity_toolbox_entity_group_attach_info().
 * @see hook_entity_toolbox_field_category_group_info().
 */
function hook_entity_toolbox_entity_info() {
  $info            = array();
  $info['product'] = array(
	'entity_type'   => 'product',
	'base_table'    => 'product',
	'fieldable'     => TRUE,
	'bundle_entity' => 'product_type',
	'module'        => 'hedios_catalog',
	'exportable'    => FALSE,
	'paths'         => array(
	  'root'   => 'admin/hedios/catalog',
	  'admin'  => 'products',
	  'manage' => 'edit'
	),
	'has_revisions' => FALSE,
	'translatable'  => FALSE,
	'properties'    => array(
	  'product_category' => array(
		'type'          => 'parent',
		'reference'     => 'product_category',
		'schema field'  => 'parent_product_category_id',
		'title'         => t('Product category'),
		'description'   => t('The product category.'),
		'required'      => array(
		  'create'    => FALSE,
		  'edit_form' => TRUE
		),
		'translatable'  => FALSE,
		'callbacks'     => array(
		  'getter'              => 'custom_getter',
		  'setter'              => 'custom_setter',
		  'validation callback' => 'custom_validation',
		  'access callback'     => 'custom_access',
		),
		'permissions'   => array(
		  'getter' => 'custom_getter_permission',
		  'setter' => 'custom_setter_permission',
		  'access' => 'custom_access_permission',
		),
		'expose'        => array(
		  'views'     => TRUE,
		  'edit_form' => TRUE,
		),
		'field'         => array(
		  'widget'       => 'list',
		  'options_list' => 'options_callback'
		),
		'clear'         => array(),
		'views_display' => array(
		  'admin' => TRUE
		)
	  ),
	),
	'id_property'   => 'product_id',
	'bundles'       => array(),
	'group'         => ''
  );
  return $info;
}

/**
 * Returns an array of updated/added/removed properties for an entity type.
 *
 *  - action (string) : the update action.
 *    Possible options are :
 *    - add
 *    - update
 *    - delete
 *
 * @return array
 */
function hook_entity_toolbox_ENTITY_TYPE_properties_update_N() {
  $update                             = array();
  $update['parent_product_family_id'] = array(
	'action'       => 'add',
	'key'          => 'product_family',
	'is_key'       => TRUE,
	'title'        => 'Product family',
	'description'  => 'The product family.',
	'is_parent'    => TRUE,
	'type'         => 'reference',
	'source'       => 'product',
	'multiple'     => FALSE,
	'required'     => TRUE,
	'translatable' => FALSE,
	'schema'       => array(
	  'description' => 'The product {product_family}.product_category_id - a unique identifier.',
	  'type'        => 'int',
	  'not null'    => TRUE,
	  'default'     => 0,
	),
	'expose'       => array(
	  'views'     => TRUE,
	  'edit_form' => TRUE,
	),
	'field'        => array(
	  'widget' => 'list'
	)
  );
  return $update;
}

/**
 * Returns an array containing info to create an entities group.
 * Entities group are meant to ease functions use and data formatting/validation.
 *
 * @return array
 */
function hook_entity_toolbox_entity_group_info() {
  $info            = array();
  $info['catalog'] = array(
	'label'       => t('Catalog'),
	'description' => t(''),
  );
  return $info;
}

/**
 * @return array
 */
function hook_entity_toolbox_entity_group_attach_info() {
  $info = array(
	'product_category',
	'product_family',
	'product_line',
	'product'
  );

  return array_fill_keys($info, array('group' => 'catalog'));
}

/**
 * @return array
 */
function hook_entity_toolbox_field_category_group_info() {
  $info                       = array();
  $info['catalog_attributes'] = array(
	'label'               => t('Catalog attribute categories'),
	'description'         => t(''),
	'entity_group_attach' => '',
  );
  return $info;
}

/**
 * @return array
 */
function hook_entity_toolbox_field_category_info() {
  $info              = array();
  $info['technical'] = array(
	'label'       => t('Technical'),
	'description' => t('A category for technical data, such as an ISIN code, etc...'),
	'group'       => 'catalog_attributes'
  );
  return $info;
}

/**
 * @return array
 */
function hook_entity_toolbox_property_tpl_info() {
  $info                   = array();
  $info['id']             = array(
	'key'            => 'id',
	'title'          => '%entity_type% ID',
	'description'    => 'The %entity_type% ID.',
	'type'           => 'serial',
	'required'       => TRUE,
	'schema'         => array(
	  'description' => 'Primary Key: Identifier for a $entity_type2entity_label(%entity_type%)$.',
	  'type'        => 'serial',
	  'unsigned'    => TRUE,
	  'not null'    => TRUE,
	),
	'is_parent'      => FALSE,
	'expose'         => array(
	  'views'     => TRUE,
	  'edit_form' => FALSE
	),
	'process_as_key' => TRUE,
  );
  $info['name']           = array(
	'key'            => 'label',
	'title'          => 'Name',
	'description'    => 'The %entity_type% name.',
	'schema'         => array(
	  'description' => 'The {%entity_type%}.name - a human-readable identifier.',
	  'type'        => 'varchar',
	  'length'      => 255,
	  'not null'    => TRUE,
	  'default'     => '',
	),
	'type'           => 'string',
	'required'       => TRUE,
	'is_parent'      => FALSE,
	'expose'         => array(
	  'views'     => TRUE,
	  'edit_form' => TRUE
	),
	'process_as_key' => TRUE,
	'field'          => array(
	  'widget' => 'textfield',
	  'params' => array(
		'#max_length' => 255
	  )
	),
  );
  $info['type']           = array(
	'key'            => 'bundle',
	'title'          => 'Type',
	'label'          => 'The $entity_type2entity_label(%entity_type%)$ bundle.',
	'schema'         => array(
	  'description' => 'The {%entity_type%}.type of this %entity_type%.',
	  'type'        => 'varchar',
	  'length'      => 255,
	  'not null'    => TRUE,
	  'default'     => '',
	),
	'type'           => 'string',
	'required'       => TRUE,
	'is_parent'      => FALSE,
	'expose'         => array(
	  'views'     => TRUE,
	  'edit_form' => FALSE
	),
	'process_as_key' => TRUE,
	'field'          => array(),
  );
  $info['status']         = array(
	'key'            => 'status',
	'title'          => 'Published',
	'label'          => 'The current publish status for this $entity_type2entity_label(%entity_type%)$.',
	'schema'         => array(
	  'description' => 'The current publish status for this %entity_type%.',
	  'type'        => 'int',
	  'not null'    => TRUE,
	  'default'     => 1
	),
	'type'           => 'boolean',
	'required'       => TRUE,
	'is_parent'      => FALSE,
	'expose'         => array(
	  'views'     => TRUE,
	  'edit_form' => TRUE
	),
	'process_as_key' => TRUE,
	'field'          => array(
	  'widget' => 'boolean_checkbox',
	  'params' => array(
		'#default_value' => 1
	  )
	),
  );
  $info['release_status'] = array(
	'key'            => 'status',
	'title'          => 'Active',
	'label'          => 'The current release status for this $entity_type2entity_label(%entity_type%)$.',
	'schema'         => array(
	  'description' => 'The current release status for this %entity_type%.',
	  'type'        => 'int',
	  'not null'    => TRUE,
	  'default'     => 1
	),
	'type'           => 'boolean',
	'required'       => TRUE,
	'is_parent'      => FALSE,
	'expose'         => array(
	  'views'     => TRUE,
	  'edit_form' => TRUE
	),
	'process_as_key' => TRUE,
	'field'          => array(
	  'widget' => 'boolean_checkbox',
	  'params' => array(
		'#default_value' => 1
	  )
	),
  );
  return $info;
}

/**
 * @return array
 */
function hook_permission_type_info() {
  $info               = array();
  $info['administer'] = array();
  $info['create']     = array();
  $info['edit']       = array();
  $info['view']       = array();
  $info['delete']     = array();
  return $info;
}