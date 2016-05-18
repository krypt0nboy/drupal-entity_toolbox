<?php

/**
 * @file
 */

/**
 * Transforms the entity_type machine name to a human readable label.
 * (Eg : "foo_bar" turns to "Foo bar").
 *
 * @param $entity_type
 * @return string
 */
function entity_type2entity_label($entity_type) {
  $split = explode('_', $entity_type);
  $label = '';
  for ($i = 0; $i < count($split); $i++) {
	if ($i == 0) {
	  $label .= ucfirst($split[$i]) . ' ';
	}
	else {
	  if ($i + 1 == count($split)) {
		$label .= $split[$i];
	  }
	  else {
		$label .= $split[$i] . ' ';
	  }
	}
  }

  return $label;
}

/**
 * Transforms the entity_type single machine name to a human readable plural label.
 * (Eg : "foo_city" turns to "Foo cities").
 *
 * @param $entity_type
 *
 * @return string
 */
function entity_type2entity_label_plural($entity_type) {
  $split = explode('_', $entity_type);
  $label = '';
  switch (substr(end($split), (strlen(end($split)) - 1), 1)) {
	case 's':
	  $end = end($split) . 'es';
	  break;
	case 'y':
	  $end = substr_replace(end($split), 'ies', (strlen(end($split)) - 1), 1);
	  break;
	default:
	  $end = end($split) . 's';
	  break;
  }
  $split[count($split) - 1] = $end;
  if (count($split) == 1) {
	return ucfirst(reset($split));
  }
  else {
	if (count($split) > 1) {
	  reset($split);
	  for ($i = 0; $i < count($split); $i++) {
		switch ($i) {
		  case 0:
			$label .= ucfirst($split[$i]) . ' ';
			break;
		  case (count($split) - 1):
			$label .= $split[$i];
			break;
		  default:
			$label .= $split[$i] . ' ';
			break;
		}
	  }
	}
  }

  return $label;
}

/**
 * Transforms the entity_type machine_name into the entity class name.
 * (Eg : "foo_bar" turns to "FooBar").
 *
 * @param $entity_type
 * @return string
 */
function entity_type2entity_class($entity_type) {
  $split = explode('_', $entity_type);
  $class = '';
  for ($i = 0; $i < count($split); $i++) {
	$class .= ucfirst($split[$i]);
  }
  return $class;
}

/**
 * Transforms the entity_type machine_name into the entity controller class name.
 * (Eg : "foo_bar_type" turns to "FooBarTypeController").
 *
 * @param $entity_type
 * @return string
 */
function entity_type2entity_controller_class($entity_type) {
  $split = explode('_', $entity_type);
  $class = '';
  for ($i = 0; $i < count($split); $i++) {
	$class .= ucfirst($split[$i]);
  }
  return $class . 'Controller';
}

/**
 * Transforms the entity_type machine_name into the entity controller class name.
 * (Eg : "foo_bar_type" turns to "FooBarTypeUIController").
 *
 * @param $entity_type
 * @return string
 */
function entity_type2entity_ui_controller_class($entity_type) {
  $split = explode('_', $entity_type);
  $class = '';
  for ($i = 0; $i < count($split); $i++) {
	$class .= ucfirst($split[$i]);
  }
  return $class . 'UIController';
}

/**
 * @param $entity_type
 * @param $module
 * @return string
 */
function entity_toolbox_default_admin_file($entity_type, $module) {
  return $module . '.' . $entity_type . '.admin.inc';
}

/**
 * Transforms the entity_type single machine_name into the entity plural machine_name.
 * (Eg : "foo_bar" turns to "foo_bars").
 *
 * @param $machine_name
 * @return mixed|string
 */
function entity_machine_name_single2plural($machine_name) {
  switch (substr($machine_name, (strlen($machine_name) - 1), 1)) {
	case 's':
	  return $machine_name . 'es';
	  break;
	case 'y':
	  return substr_replace($machine_name, 'ies', (strlen($machine_name) - 1), 1);
	  break;
	default:
	  return $machine_name . 's';
	  break;
  }
}

/**
 * @param $key
 * @return string
 */
function entity_key2field_title($key) {
  return ucfirst(preg_replace('/_/', ' ', $key));
}

/**
 * Replaces one or more tokens in a string with their matching values.
 *
 * @param      $string
 *   The string to filter.
 * @param      $data
 *   An array to get the matching data from.
 *
 * @return string
 */
function string_replace_matching_tokens($string, $data) {
  if ((bool) preg_match_all('/(\$\w{1,}\(%\w{1,}%\)\$)/', $string, $matches)) {
	$matches = $matches[0];
	for ($r = 0; $r < count($matches); $r++) {
	  preg_match('/\$(\w{1,})\(/', $matches[$r], $f_matches);
	  preg_match('/\$\w{1,}\(%(\w{1,})%\)/', $matches[$r], $t_matches);
	  $function = $f_matches[1];
	  $token    = $t_matches[1];
	  if (is_callable($function)) {
		$replace_value = $function($data[$token]);
		$pos           = strpos($string, $matches[$r]);
		$lenght        = strlen($matches[$r]);
		$_r            = $pos + $lenght;
		$string        = str_replace($matches[$r], $replace_value, $string);
	  }
	}
  }
  else {
	if ((bool) preg_match_all('/(\$\[%\w{1,}%\]->%\w{1,}%\$)/', $string, $matches)) {
	  $matches = $matches[0];
	  for ($r = 0; $r < count($matches); $r++) {
	  }
	}
	else {
	  if ((bool) preg_match_all('/(%\w{1,}%)/', $string, $matches)) {

	  }
	}
  }

  return $string;
}