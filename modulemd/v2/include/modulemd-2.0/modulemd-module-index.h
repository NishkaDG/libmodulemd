/*
 * This file is part of libmodulemd
 * Copyright (C) 2018 Red Hat, Inc.
 *
 * Fedora-License-Identifier: MIT
 * SPDX-2.0-License-Identifier: MIT
 * SPDX-3.0-License-Identifier: MIT
 *
 * This program is free software.
 * For more information on the license, see COPYING.
 * For more information on free software, see <https://www.gnu.org/philosophy/free-sw.en.html>.
 */

#pragma once

#include <glib-object.h>
#include "modulemd-module.h"
#include "modulemd-translation.h"
#include "modulemd-subdocument-info.h"

G_BEGIN_DECLS

/**
 * SECTION: modulemd-module-index
 * @title: Modulemd.ModuleIndex
 * @stability: stable
 * @short_description: The primary interface to dealing with repodata in the
 * module metadata format.
 *
 * This object provides an interface to the complete metadata read from a
 * repository or manually added to this object.
 *
 * NOTE: When adding or updating this object from YAML, all objects imported
 * will be automatically upgraded to match the highest version of that object
 * that is seen. This means that if the repository has a mix of
 * #ModulemdModuleStreamV1 and #ModulemdModuleStreamV2 objects, the index will
 * contain only #ModulemdModuleStreamV2. You can check the versions the index
 * upgraded to with the modulemd_module_index_get_stream_mdversion() and
 * modulemd_module_index_get_defaults_mdversion(). If your application would
 * prefer to always work with a particular stream or defaults version (such as
 * to avoid extra branching logic), the modulemd_module_index_upgrade_streams()
 * and modulemd_module_index_upgrade_defaults() functions can be used to force
 * the contents of the index to upgrade to those versions.
 *
 * Interacting with #ModulemdModuleIndex is relatively simple. A common Python
 * example for working with Fedora repodata might be (assuming the metadata has
 * already been read into strings):
 *
 * |[<!-- language="Python" -->
 * fedora_repo_index = Modulemd.ModuleIndex.new()
 * fedora_repo_index.update_from_string(fedora_modulemd)
 *
 * # Get the list of all module names in the index
 * module_names = fedora_repo_index.get_module_names()
 *
 * # Retrieve information about a particular module from the index
 * module = fedora_repo_index.get_module('module_name')
 * ]|
 *
 * See the #ModulemdModule documentation for details on retrieving information
 * about specific modules, including how to get #ModulemdDefaults information
 * and retrieve individual #ModulemdModuleStream objects.
 *
 * See the #ModulemdModuleIndexMerger documentation for details on merging
 * #ModulemdModuleIndex objects from separate repositories together.
 */

#define MODULEMD_TYPE_MODULE_INDEX (modulemd_module_index_get_type ())

G_DECLARE_FINAL_TYPE (
  ModulemdModuleIndex, modulemd_module_index, MODULEMD, MODULE_INDEX, GObject)


/**
 * modulemd_module_index_new:
 *
 * Returns: (transfer full): A newly-allocated #ModulemdModuleIndex object.
 *
 * Since: 2.0
 */
ModulemdModuleIndex *
modulemd_module_index_new (void);


/**
 * modulemd_module_index_update_from_file:
 * @self: This #ModulemdModuleIndex object
 * @yaml_file: (in): A YAML file containing the module metadata and other
 * related information such as default streams.
 * @strict: (in): Whether the parser should return failure if it encounters an
 * unknown mapping key or if it should ignore it.
 * @failures: (out) (element-type ModulemdSubdocumentInfo) (transfer container):
 * An array containing any subdocuments from the YAML file that failed to parse.
 * See #ModulemdSubdocumentInfo for more details.
 * @error: (out): A GError containing additional information if this function
 * fails in a way that prevents program continuation.
 *
 * Returns: TRUE if the update was successful. Returns FALSE and sets failures
 * approriately if any of the YAML subdocuments were invalid or sets @error if
 * there was a fatal parse error.
 *
 * Since: 2.0
 */
gboolean
modulemd_module_index_update_from_file (ModulemdModuleIndex *self,
                                        const gchar *yaml_file,
                                        gboolean strict,
                                        GPtrArray **failures,
                                        GError **error);


/**
 * modulemd_module_index_update_from_string:
 * @self: This #ModulemdModuleIndex object
 * @yaml_string: (in): A YAML string containing the module metadata and other
 * related information such as default streams.
 * @strict: (in): Whether the parser should return failure if it encounters an
 * unknown mapping key or if it should ignore it.
 * @failures: (out) (element-type ModulemdSubdocumentInfo) (transfer container):
 * An array containing any subdocuments from the YAML file that failed to parse.
 * See #ModulemdSubdocumentInfo for more details.
 * @error: (out): A GError containing additional information if this function
 * fails in a way that prevents program continuation.
 *
 * Returns: TRUE if the update was successful. Returns FALSE and sets failures
 * approriately if any of the YAML subdocuments were invalid or sets @error if
 * there was a fatal parse error.
 *
 * Since: 2.0
 */
gboolean
modulemd_module_index_update_from_string (ModulemdModuleIndex *self,
                                          const gchar *yaml_string,
                                          gboolean strict,
                                          GPtrArray **failures,
                                          GError **error);


/**
 * modulemd_module_index_update_from_stream: (skip)
 * @self: This #ModulemdModuleIndex object
 * @yaml_stream: (in): A YAML stream containing the module metadata and other
 * related information such as default streams.
 * @strict: (in): Whether the parser should return failure if it encounters an
 * unknown mapping key or if it should ignore it.
 * @failures: (out) (element-type ModulemdSubdocumentInfo) (transfer container):
 * An array containing any subdocuments from the YAML file that failed to parse.
 * See #ModulemdSubdocumentInfo for more details.
 * @error: (out): A GError containing additional information if this function
 * fails in a way that prevents program continuation.
 *
 * Returns: TRUE if the update was successful. Returns FALSE and sets failures
 * approriately if any of the YAML subdocuments were invalid or sets @error if
 * there was a fatal parse error.
 *
 * Since: 2.0
 */
gboolean
modulemd_module_index_update_from_stream (ModulemdModuleIndex *self,
                                          FILE *yaml_stream,
                                          gboolean strict,
                                          GPtrArray **failures,
                                          GError **error);


/**
 * modulemd_module_index_dump_to_string:
 * @self: This #ModulemdModuleIndex
 * @error: (out): A #GError containing the reason the function failed, NULL if
 * the function succeeded.
 *
 * Returns: (transfer full): A YAML representation of the index as a string. In
 * the event of an error, sets error appropriately and returns NULL.
 *
 * Since: 2.0
 */
gchar *
modulemd_module_index_dump_to_string (ModulemdModuleIndex *self,
                                      GError **error);

/**
 * modulemd_module_index_dump_to_stream: (skip)
 * @self: This #ModulemdModuleIndex
 * @yaml_stream: (in): A stream to write the module metadata and other related
 * information to.
 * @error: (out): A #GError containing the reason the function failed, NULL if
 * the function succeeded.
 *
 * Returns: TRUE if written successfully, FALSE and sets error appropriately in
 * the event of an error.
 *
 * Since: 2.0
 */
gboolean
modulemd_module_index_dump_to_stream (ModulemdModuleIndex *self,
                                      FILE *yaml_stream,
                                      GError **error);


/**
 * modulemd_module_index_get_module_names_as_strv: (rename-to modulemd_module_index_get_module_names)
 * @self: This #ModulemdModuleIndex
 *
 * Returns: (transfer full): An ordered list of string keys in this index.
 *
 * Since: 2.0
 */
GStrv
modulemd_module_index_get_module_names_as_strv (ModulemdModuleIndex *self);


/**
 * modulemd_module_index_get_module:
 * @self: This #ModulemdModuleIndex
 * @module_name: The module name to look up in the index.
 *
 * Returns: (transfer none): The #ModulemdModule object matching the provided
 * module name or NULL if the key was not present in the index.
 *
 * Since: 2.0
 */
ModulemdModule *
modulemd_module_index_get_module (ModulemdModuleIndex *self,
                                  const gchar *module_name);


/**
 * modulemd_module_index_remove_module:
 * @self: This #ModulemdModuleIndex
 * @module_name: The name of the module to remove from the index.
 *
 * Remove a module, including all of its streams, its defaults and its
 * translations from a #ModulemdModuleIndex.
 *
 * Returns: TRUE if the module name was present in the index. FALSE if it was
 * not.
 *
 * Since: 2.2
 */

gboolean
modulemd_module_index_remove_module (ModulemdModuleIndex *self,
                                     const gchar *module_name);

/**
 * modulemd_module_index_add_module_stream:
 * @self: This #ModulemdModuleIndex
 * @stream: The #ModulemdModuleStream to add to the index. The stream added
 * must have a module name and stream name set on it or it will be rejected.
 * @error: (out): A #GError containing the reason the #ModulemdModuleStream
 * object could not be added or NULL if the function succeeded.
 *
 * Add a #ModuleStream to the #ModuleIndex. While being added, the
 * #ModuleStream will be upgraded to MD_MODULESTREAM_VERSION_LATEST to ensure
 * that merges done with #ModulemdModuleIndexMerger will always occur between
 * streams of the same version. If this upgrade cannot be performed, the
 * function will return @error set appropriately.
 *
 * Returns: TRUE if the #ModulemdModule was added succesfully. If the stream
 * already existed in the index, it will be replaced by the new one. On
 * failure, returns FALSE and sets @error appropriately.
 *
 * Since: 2.0
 */
gboolean
modulemd_module_index_add_module_stream (ModulemdModuleIndex *self,
                                         ModulemdModuleStream *stream,
                                         GError **error);


/**
 * modulemd_module_index_add_defaults:
 * @self: This #ModulemdModuleIndex
 * @defaults: The #ModulemdDefaults object to add to the index.
 * @error: (out): A #GError containing the reason the #ModulemdDefaults object
 * could not be added or NULL if the function succeeded.
 *
 * Returns: TRUE if the #ModulemdDefaults was added succesfully. If the defaults
 * already existed in the index, it will be replaced by the new one. On failure,
 * returns FALSE and sets error approriately.
 *
 * Since: 2.0
 */
gboolean
modulemd_module_index_add_defaults (ModulemdModuleIndex *self,
                                    ModulemdDefaults *defaults,
                                    GError **error);


/**
 * modulemd_index_add_translation:
 * @self: This #ModulemdModuleIndex
 * @translation: The #ModulemdTranslation object to add to the index.
 * @error: (out): A #GError containing the reason the #ModulemdTranslation
 * object could not be added or NULL if the function succeeded.
 *
 * Returns: TRUE if the #ModulemdTranslation was added succesfully. If the
 * translation already existed in the index, it will be replaced by the new one.
 * On failure, returns FALSE and sets error appropriately.
 *
 * Since: 2.0
 */
gboolean
modulemd_module_index_add_translation (ModulemdModuleIndex *self,
                                       ModulemdTranslation *translation,
                                       GError **error);

/**
 * modulemd_module_index_get_defaults_mdversion:
 * @self: This #ModulemdModuleIndex
 *
 * Returns: The metadata version of #ModulemdDefaults in use for this index.
 *
 * Since: 2.0
 */
ModulemdDefaultsVersionEnum
modulemd_module_index_get_defaults_mdversion (ModulemdModuleIndex *self);


/**
 * modulemd_module_index_get_stream_mdversion:
 * @self: This #ModulemdModuleIndex
 *
 * Returns: The metadata version of #ModulemdModuleStream in use for this
 * index.
 *
 * Since: 2.0
 */
ModulemdModuleStreamVersionEnum
modulemd_module_index_get_stream_mdversion (ModulemdModuleIndex *self);


/**
 * modulemd_module_index_upgrade_streams:
 * @self: This #ModulemdModuleIndex
 * @mdversion: The #ModulemdModuleStream metadata version to upgrade to.
 * @error: (out): A #GError that contains information on why the index could
 * not be upgraded in the event of an error.
 *
 * Upgrades all #ModulemdModuleStream objects in this index to @mdversion if
 * they are not already at that version.
 *
 * Since: 2.0
 */
gboolean
modulemd_module_index_upgrade_streams (
  ModulemdModuleIndex *self,
  ModulemdModuleStreamVersionEnum mdversion,
  GError **error);


/**
 * modulemd_module_index_upgrade_defaults:
 * @self: This #ModulemdModuleIndex
 * @mdversion: The #ModulemdDefaults metadata version to upgrade to.
 * @error: (out): A #GError that contains information on why the index could
 * not be upgraded in the event of an error.
 *
 * Upgrades all #ModulemdDefaults objects in this index to @mdversion if they
 * are not already at that version.
 *
 * Since: 2.0
 */
gboolean
modulemd_module_index_upgrade_defaults (ModulemdModuleIndex *self,
                                        ModulemdDefaultsVersionEnum mdversion,
                                        GError **error);

G_END_DECLS
