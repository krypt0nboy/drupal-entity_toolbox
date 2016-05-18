<?php

/**
 * Class entity_toolbox_handler_link_field
 */
class entity_toolbox_handler_link_field extends views_handler_field {

  /**
   * @var
   */
  public $entityType;
  public $entityToolboxInfo = array();

  /**
   * Constructor.
   */
  public function construct() {
	parent::construct();
	if (!empty($this->definition['type'])) {
	  $this->entityType                                                 = $this->definition['type'];
	  $this->entityToolboxInfo                                          = entity_toolbox_entity_get_info($this->entityType);
	  $this->additional_fields[$this->entityToolboxInfo['id_property']] = $this->entityToolboxInfo['id_property'];
	  $this->additional_fields['type']                                  = 'type';
	}
  }

  /**
   * Get option definition.
   *
   * @return array
   *   Definition.
   */
  public function option_definition() {
	$options = parent::option_definition();

	$options['text'] = array('default' => '', 'translatable' => TRUE);

	return $options;
  }

  /**
   * Update form field.
   *
   * @param array $form
   *   Form.
   * @param array $form_state
   *   Form state.
   */
  public function options_form(&$form, &$form_state) {
	parent::options_form($form, $form_state);

	$form['text'] = array(
	  '#type'          => 'textfield',
	  '#title'         => t('Text to display'),
	  '#default_value' => $this->options['text'],
	);
  }

  /**
   * Update query.
   */
  public function query() {
	$this->ensure_my_table();
	$this->add_additional_fields();
  }

  /**
   * Render link.
   *
   * @param array $values
   *   Values.
   *
   * @return string
   *   Link.
   */
  public function render($values) {
	$text      = !empty($this->options['text']) ? $this->options['text'] : t('View');
	$entity_id = $values->{$this->aliases[$this->entityToolboxInfo['id_property']]};

	return l($text, $this->entityType . '/' . $entity_id);
  }
}

/**
 * Class entity_toolbox_handler_edit_link_field
 */
class entity_toolbox_handler_edit_link_field extends entity_toolbox_handler_link_field {

  /**
   * Constructor.
   */
  public function construct() {
	parent::construct();
  }

  /**
   * Render link.
   *
   * @param array $values
   *   Values.
   *
   * @return string
   *   Link.
   */
  public function render($values) {
	$type = $values->{$this->aliases['type']};

	// Creating a dummy entity to check access against.
	$dummy_entity = (object) array('type' => $type);
	if (!entity_access('edit', $this->entityType, $dummy_entity)) {
	  return;
	}

	$text      = !empty($this->options['text']) ? $this->options['text'] : t('Edit');
	$entity_id = $values->{$this->aliases[$this->entityToolboxInfo['id_property']]};

	return l($text, entity_toolbox_entity_get_path($this->entityType, 'admin') . '/' . $this->entityType . '/' . $entity_id . '/edit');
  }
}

/**
 * Class entity_toolbox_handler_delete_link_field.
 */
class entity_toolbox_handler_delete_link_field extends entity_toolbox_handler_link_field {
  /**
   * Constructor.
   */
  public function construct() {
	parent::construct();
  }

  /**
   * Render link.
   *
   * @param array $values
   *   Values.
   *
   * @return string
   *   Link.
   */
  public function render($values) {
	$type = $values->{$this->aliases['type']};

	// Creating a dummy entity to check access against.
	$dummy_entity = (object) array('type' => $type);
	if (!entity_access('edit', $this->entityType, $dummy_entity)) {
	  return;
	}

	$text      = !empty($this->options['text']) ? $this->options['text'] : t('Delete');
	$entity_id = $values->{$this->aliases[$this->entityToolboxInfo['id_property']]};

	return l($text, entity_toolbox_entity_get_path($this->entityType, 'admin') . '/' . $this->entityType . '/' . $entity_id . '/delete');
  }
}

/**
 * Class entity_toolbox_handler_entity_operations_field.
 */
class entity_toolbox_handler_entity_operations_field extends views_handler_field {

  public $entityType;
  public $entityToolboxInfo = array();

  /**
   * Constructor.
   */
  public function construct() {
	parent::construct();
	if (!empty($this->definition['type'])) {
	  $this->entityType                                                 = $this->definition['type'];
	  $this->entityToolboxInfo                                          = entity_toolbox_entity_get_info($this->entityType);
	  $this->additional_fields[$this->entityToolboxInfo['id_property']] = $this->entityToolboxInfo['id_property'];
	  $this->additional_fields['type']                                  = 'type';
	}
  }

  /**
   * Update query.
   */
  public function query() {
	$this->ensure_my_table();
	$this->add_additional_fields();
  }

  /**
   * Build link.
   *
   * @param array $values
   *   Values.
   *
   * @return string
   *   Link.
   *
   * @throws \Exception
   */
  public function render($values) {
	$module = $this->entityToolboxInfo['module'];
	$links  = menu_contextual_links(
	  $module,
	  entity_toolbox_entity_get_path($this->entityType, 'admin') . '/' . $this->entityType,
	  array($this->get_value($values, $this->entityToolboxInfo['id_property']))
	);

	if (!empty($links)) {
	  return theme(
		'links',
		array(
		  'links'      => $links,
		  'attributes' => array(
			'class' => array('links', 'inline', 'operations'),
		  ),
		)
	  );
	}
  }
}

