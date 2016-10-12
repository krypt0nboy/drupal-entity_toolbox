<?php

/**
 * Class EntityToolboxOperationsProperties
 */
trait EntityToolboxOperationsProperties {
  /**
   * @access protected
   * @var string $basePath
   *   The operation base path.
   */
  protected $basePath;
  /**
   * @access protected
   * @var string $activeRedirectPath
   *   The active redirect path.
   */
  protected $activeRedirectPath;
  /**
   * @access protected
   * @var array $availableRedirects
   *   The available redirect paths.
   */
  protected $availableRedirects = array();
}