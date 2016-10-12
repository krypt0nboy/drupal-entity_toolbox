<?php

/**
 * @file
 *
 * This file contains validation and translation functions required by entity_toolbox.
 */

/**
 * Returns the type of an entity.
 *
 * @param \Entity $entity
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
 * @param \Entity $entity
 *  The entity.
 *
 * @return string
 */
function entity_bundle($entity) {
  return $entity->bundle();
}

/**
 * Returns the author of the entity.
 *
 * @param \Entity|\EntityToolbox $entity
 *   The entity.
 *
 * @return Entity
 */
function entity_author($entity) {
  //@todo add entity_type_is_entity_toolbox()
  $entity_type = $entity->entityType();
  if (in_array($entity_type, entity_toolbox_get_info())) {
    $controller = entity_toolbox_controller($entity_type);

    return $controller->getAuthor($entity);
  }
  else {
    return user_load($entity->uid);
  }
}

/**
 * Returns the entity author uid.
 *
 * @param \Entity|\EntityToolbox $entity
 *   The entity.
 *
 * @return int
 */
function entity_author_id($entity) {
//@todo add entity_type_is_entity_toolbox()
  $entity_type = $entity->entityType();
  if (in_array($entity_type, entity_toolbox_get_info())) {
    $controller = entity_toolbox_controller($entity_type);

    return $controller->getAuthorUid($entity);
  }
  else {
    return $entity->uid;
  }
}

/**
 * Returns an entity wrapper object.
 * No need to specify the entity type.
 *
 * @param \Entity $entity
 *   The entity to get the wrapper for.
 *
 * @return \EntityMetadataWrapper
 */
function entity_wrapper($entity) {
  return entity_metadata_wrapper(entity_type($entity), $entity);
}

/**
 * Returns every property type declared and allowed by entity_toolbox.
 *
 * @return array
 */
function toolbox_property_types() {
  return array_keys(entity_toolbox_property_type_get_info());
}

/**
 * Returns the system path of an entity_type.
 *
 * @param string $entity_type
 *   The entity type.
 * @param string $alias
 *   The menu item alias.
 *
 * @return string
 */
function entity_get_path($entity_type, $alias) {
}

/**
 * Returns the form type of a form.
 * Eg : "my_entity_edit_form" will return "edit".
 *
 * @param string $form_id
 *   The form ID.
 *
 * @return string
 */
function entity_form_extract_type_from_id($form_id) {
  $regexp = '/^(?P<entity_type>.+)_(?P<type>.+)_form$/';
  if (preg_match($regexp, $form_id, $matches)) {
    return $matches['type'];
  }
}

/**
 * Returns REQUEST_TIME.
 *
 * @return int
 */
function request_time() {
  return REQUEST_TIME;
}

/**
 * Returns the current $user->uid.
 *
 * @return int
 */
function user_id() {
  global $user;

  return $user->uid;
}

/**
 * @param $entity_type
 * @return array
 */
function entity_type_get_parents($entity_type) {

}

/**
 * Returns a list of ids from a list of references.
 *
 * @param array  $references
 *   The references list.
 * @param string $reference_type
 *   The references type.
 *
 * @return integer[]
 */
function references2references_ids(array $references = array(), $reference_type) {
  if (is_array_integers($references)) {
    return $references;
  }
  else {
    $ids         = array();
    $entity_info = entity_get_info($reference_type);
    $key_id      = $entity_info['entity keys']['id'];
    foreach ($references as $key => $reference) {
      $ids[] = $reference->{$key_id};
    }

    return $ids;
  }
}

/**
 * Translates an arg type.
 *
 * @param string $type
 *   The type to translate.
 *
 * @return string
 */
function translate_type($type) {
  return preg_match('/\|/', $type) ? 'mixed' : $type;
}

/**
 * Extract the real arg types.
 *
 * @param string $type
 *   The type to extract.
 *
 * @return array
 */
function extract_types($type) {
  return explode('|', $type);
}

/**
 * @param $var
 * @return int
 */
function is_template_value($var) {
  return preg_match(RGX__WILDCARD, $var);
}

/**
 * Returns a list with the entity type as key and the label as value.
 *
 * @param array $types
 *   The types to list.
 *
 * @return array
 */
function entity_types_label_list(array $types = array()) {
  $list = array();
  foreach ($types as $type) {
    $list[$type] = entity_type_label_single($type);
  }

  return $list;
}

function entity_bundles_list($entity_type) {
  $list     = array();
  $function = entity_type_callbacks($entity_type, 'get_types');
  foreach ($function() as $type) {
    $list[$type->type] = $type->label;
  }

  return $list;
}