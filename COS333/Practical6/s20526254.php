<?php
  $filename = $argv[1];
  $searchWord = $argv[2];

  if (!file_exists($filename)) {
    echo "File not found.\n";
    exit(1);
  }

  $content = file_get_contents($filename);
  echo "Original File Contents:\n";
  echo "-----------------------\n";
  echo "$content";
  echo "-----------------------\n";
  $content = strtolower($content);
  $content = preg_replace('/[^a-z]/', '', $content);
  echo "\nFile Contents after conversion: $content\n";

  $reversedContent = strrev($content);
  $matches = substr_count($reversedContent, $searchWord);

  echo "Search Word: $searchWord\n";
  echo "Matches: $matches\n";
?>
