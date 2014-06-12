//=============================================================================
//	This library is free software; you can redistribute it and/or modify it
//	under the terms of the GNU Library General Public License as published
//	by the Free Software Foundation; either version 2 of the License, or
//	(at your option) any later version.
//
//	This library is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//	Library General Public License for more details.
//
//	The GNU Public License is available in the file LICENSE, or you
//	can write to the Free Software Foundation, Inc., 59 Temple Place -
//	Suite 330, Boston, MA 02111-1307, USA, or you can find it on the
//	World Wide Web at http://www.fsf.org.
//=============================================================================
/** \file
 * \author Josh Staley
 * \brief Useful extension functions for Variant
 */

#ifndef VARIANT_EXTENSIONS_H
#define VARIANT_EXTENSIONS_H

#include <Variant/Variant.h>

namespace libvariant {

	/// \brief Returns a default parameters for use with FlattenPath and ExpandPath functions.
	Variant ExtensionParams();

	/// \brief Sets any missing Extension parameters to 'params' for use with FlattenPath and ExpandPath functions.
	void ExtensionParams( VariantRef params );

	/// \brief Returns a version of 'var' with flat paths based on 'params'
	Variant FlattenPath( const Variant &var, Variant params = ExtensionParams() );

	/// \brief Returns a version of 'var' with expanded paths based on 'params'
	Variant ExpandPath( const Variant &var, Variant params = ExtensionParams() );

	/// \brief Flattens 'var' paths to 'flat' based on 'params'
	void FlattenPathTo( VariantRef flat, const Variant &var, Variant params = ExtensionParams(), std::string prefix = "" );

	/// \brief Expands 'var' paths to 'expand' based on 'params'
	void ExpandPathTo( VariantRef expand, const Variant &var, Variant params = ExtensionParams() );

	/// \brief Returns a string string map representation of 'var' based on 'params'
	std::map<std::string,std::string>
	FlattenPathToStrStrMap( const Variant &var, Variant params = ExtensionParams() );

	/// \brief Returns a variant representing elements in 'compare' that differ from 'base' recursively
	Variant RDiff( const Variant &base, const Variant &compare );

	/// \brief Sets elements into 'diff' from 'compare' that differ from 'base' recursively
	void RDiff( VariantRef diff, const Variant &base, const Variant &compare );

	/// \brief Updates 'base' with entries in 'update' recursively
	void RUpdate( VariantRef base, const Variant &update );

} // namespace libvariant

#endif /*#ifndef VARIANT_EXTENSIONS_H*/
