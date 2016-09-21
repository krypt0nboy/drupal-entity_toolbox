<?php

/**
 * Class EntityToolboxPropertiesDependentTraits
 */
trait EntityToolboxPropertiesDependentTraits {
  /**
   * @var array $properties
   *   The properties within toolbox info.
   */
  protected $properties = array();
  /**
   * @var array $inheritedProperties
   *   An associative array whose keys are the properties to inherit from parent
   *   entities, and the values the parent(s) from which to inherit the property.
   */
  protected $inheritedProperties = array();
  /**
   * @var array $inheritableProperties
   *   An array of inheritable properties.
   */
  protected $inheritableProperties = array();
  /**
   * @var array $referenceProperties
   *   An array of strings containing the reference property names.
   */
  protected $referenceProperties = array();
  /**
   * @var ToolboxPropertiesHelper $propsHelper
   *   A helper for the entity type properties.
   */
  protected $propsHelper;

  /**
   * @return array
   */
  public function getProperties() {
    return $this->properties;
  }

  /**
   * @param array $properties
   */
  public function setProperties($properties) {
    $this->properties = $properties;
  }

  /**
   * @return array
   */
  public function getInheritedProperties() {
    return $this->inheritedProperties;
  }

  /**
   * Retrieves the entity type inherited properties.
   *
   * @param array $inheritedProperties
   *   The inherited properties.
   */
  public function setInheritedProperties($inheritedProperties) {
    $this->inheritedProperties = $inheritedProperties;
  }

  /**
   * @return array
   */
  public function getInheritableProperties() {
    return $this->inheritableProperties;
  }

  /**
   * Retrieves the inheritable properties.
   *
   * @param array $inheritableProperties
   *   The inheritable properties.
   */
  public function setInheritableProperties($inheritableProperties) {
    $this->inheritableProperties = $inheritableProperties;
  }

  /**
   * @return array
   */
  public function getReferenceProperties() {
    return $this->referenceProperties;
  }

  /**
   * @param array $referenceProperties
   */
  public function setReferenceProperties($referenceProperties) {
    $this->referenceProperties = $referenceProperties;
  }

  /**
   * @return ToolboxPropertiesHelper
   */
  public function getPropsHelper() {
    return $this->propsHelper;
  }

  /**
   * @param ToolboxPropertiesHelper $propsHelper
   */
  public function setPropsHelper($propsHelper) {
    $this->propsHelper = $propsHelper;
  }

  /**
   * Auto sets properties.
   */
  public function autoSetProperties() {
    $this->__set('properties', $this->extractProperties());
  }

  /**
   * @return mixed
   */
  public function extractProperties() {
    return $this->toolboxInfo['properties'];
  }

  public function autoSetReferenceProperties() {

  }
}