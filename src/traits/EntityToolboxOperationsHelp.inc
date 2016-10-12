<?php

/**
 * Class EntityToolboxOperationsHelp
 */
trait EntityToolboxOperationsHelp {
  /**
   * Returns the default base path.
   *
   * @return string
   */
  public function basePath() {
    /** @var EntityToolboxOperationsAdd $this */
    $h = $this->h();

    return paths_join($h->rootPath(), array($this->getPath()));
  }
}