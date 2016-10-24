<?php
use Drupal\taxonomy\Entity\Term;

/**
 * Implements hook_drush_command().
 */
function default_content_vocabulary_drush_command() {
  $items['default-content-vocabulary-export'] = [
    'description' => dt('Exports topics for uni_edition'),
    'arguments' => [
      'entity_id' => dt('The ID of the entity to export.'),
      'module' => dt('The name of the module.'),
    ],
    'aliases' => ['dcve'],
    'required-arguments' => 1,
  ];


  return $items;
}


/**
 * Exports all the terms in a given vocabulary
 *
 * @param string $entity_type_id
 *   The entity type ID.
 * @param $entity_id
 *   The entity ID to export.
 */
function drush_default_content_vocabulary_export($entity_id, $module) {
  $module_folder = \Drupal::moduleHandler()->getModule($module_name)->getPath() . '/content';

  /** @var \Drupal\default_content\DefaultContentManagerInterface $manager */
  file_prepare_directory($module_folder);
  $manager = \Drupal::service('default_content.manager');
  $info = "default_content:\r\n  vocabulary_term:\r\n";
  $tree = \Drupal::service('entity_type.manager')
    ->getStorage("taxonomy_term")
    ->loadTree($entity_id, $parent = 0, $max_depth = NULL, $load_entities = FALSE);
  foreach ($tree as $term) {
    $term = Term::load($term->tid);
    $export = $manager->exportContent('taxonomy_term', $term->id());
    $file = $module_folder . '/' . $entity_id . '.'. $term->id() . '.json';
    file_put_contents($file, $export, FILE_USE_INCLUDE_PATH);
    $info .= "    - " . $term->uuid() . "\r\n";
  }
  drush_print($info);
}
