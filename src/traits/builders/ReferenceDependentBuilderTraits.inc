<?php

/**
 * Class ReferenceDependentBuilderTraits
 */
trait ReferenceDependentBuilderTraits {
  /**
   * Extracts the info from the property reference info.
   *
   * @return array
   */
  public function extractReferenceInfoTemplateVars() {
    $flattened = array();
    if (!empty($this->referenceInfo)) {
      $flattened = $this->extractTemplateVars($this->referenceInfo, '', 'reference_info_');
      $flattened += array(
        'reference_info_entity_type' => $this->reference,
        'reference_info_machine_name_plural' => single2plural($this->reference)
      );

    }

    return $flattened;
  }
}