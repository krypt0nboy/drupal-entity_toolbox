<?php

function theme_entity_add_list($variables) {
  $entity_type = $variables['type'];
  $content     = $variables['content'];

  if ($content) {
	$output = '<dl class="type-list">';

	foreach ($content as $item) {
	  $output .= '<dt>' . l($item['title'], $item['href']) . '</dt>';
	  $output .= '<dd>' . filter_xss_admin($item['description']) . '</dd>';
	}
	$output .= '</dl>';
  }
  else {
	$output = '<p>' . t(
		'Entities cannot be added because you have not created any types yet. Go to the <a href="@create-entity-type">Entity type creation page</a> to add a new type.',
		array('@create-entity-type' => url(entity_toolbox_entity_get_path($entity_type . '_type', 'admin') . '/add'))
	  ) . '</p>';
  }

  return $output;
}