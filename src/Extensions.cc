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
 * \brief Useful extension functions for Variants
 */

#include <sstream>
#include <stdexcept>
#include <Variant/Variant.h>
#include <Variant/Extensions.h>
#include <Variant/Path.h>

using namespace std;

#if 0
#include <iostream>
#define DBPRINT(x) cout << x
#else
#define DBPRINT(x)
#endif

namespace libvariant {

	bool SimpleList( const Variant & var ) {
		if ( !var.IsList() )
			return false;
		for ( unsigned i = 0; i < var.Size(); ++i )
			if ( var[i].IsMap() | var[i].IsList() )
				return false;
		return true;
	}

	Variant ExtensionParams() {
		Variant v;
		ExtensionParams(v);
		return v;
	}

	void ExtensionParams( VariantRef params ) {
		if ( params.IsNull() ) {
			params = Variant::MapType;
		}

		if ( !params.IsMap() ) {
			throw runtime_error("# ExtensionParams() - params must be a map");
		}

		if ( !(params.Contains("noSimpleLists") && params["noSimpleLists"].IsBool()) ) {
			params["noSimpleLists"] = true;
		}

		if ( !params.Contains("pathPrefix") ) {
			params["pathPrefix"] = "";
		}
		if ( !(params["pathPrefix"].IsString() && params["pathPrefix"].Size() <= 1) ) {
			throw runtime_error("# ExtensionParams() - Invalid pathPrefix");
		}

		if ( !params.Contains("pathDelimiter") ) {
			params["pathDelimiter"] = "/";
		}
		// Must have a delimiter
		if ( !(params["pathDelimiter"].IsString() && params["pathDelimiter"].Size() == 1) ) {
			throw runtime_error("# ExtensionParams() - Invalid pathDelimiter");
		}
	
		if ( !params.Contains("indexPrefix") ) {
			params["indexPrefix"] = "[";
		}
		// Must have a prefix
		if ( !(params["indexPrefix"].IsString() && params["indexPrefix"].Size() == 1) ) {
			throw runtime_error("# ExtensionParams() - Invalid indexPrefix");
		}

		if ( !params.Contains("indexSuffix") ) {
			params["indexSuffix"] = "]";
		}
		if ( !(params["indexSuffix"].IsString() && params["indexSuffix"].Size() <= 1) ) {
			throw runtime_error("# ExtensionParams() - Invalid indexSuffix");
		}

		if ( !params.Contains("listDelimiter") ) {
			params["listDelimiter"] = ",";
		}
		// Must have a delimiter
		if ( !(params["listDelimiter"].IsString() && params["listDelimiter"].Size() == 1)) {
			throw runtime_error("# ExtensionParams() - Invalid listDelimiter");
		}
	}

	Variant FlattenPath( const Variant &var, Variant params ) {
		Variant flat;
		FlattenPathTo( flat, var, params );
		return flat;
	}

	Variant ExpandPath( const Variant &var, Variant params ) {
		Variant expand;
		ExpandPathTo( expand, var, params );
		return expand;
	}

	void FlattenPathTo( VariantRef flat, const Variant &var, Variant params, string prefix ) {
		switch ( var.GetType() ) {
		case Variant::ListType:
			if ( (!params["noSimpleLists"].AsBool()) && SimpleList(var) ) {
				if( prefix.size() ) {
					flat[prefix] = var.Copy();
				} else {
					flat = var.Copy();
				}
			} else {
				for( unsigned i=0; i < var.Size(); ++i ) {
					stringstream tmp;
					if ( prefix.size() ) {
						tmp << prefix;
					}
					tmp << params["indexPrefix"].AsString();
					tmp << i;
					tmp << params["indexSuffix"].AsString();
					FlattenPathTo( flat, var[i], params, tmp.str() );
				}
			}
			break;
		case Variant::MapType:
			for( Variant::ConstMapIterator i(var.MapBegin()), e(var.MapEnd()); i != e; ++i ) {
				stringstream tmp;
				if ( prefix.size() ) {
					tmp << prefix + params["pathDelimiter"].AsString();
				} else {
					tmp << params["pathPrefix"].AsString();
				}
				tmp << i->first;
				FlattenPathTo( flat, i->second, params, tmp.str());
			}
			break;
		default:
			if( prefix.size() ) {
				flat[ prefix ] = var.Copy();
			} else {
				flat = var.Copy();
			}
		}
	}

	/// The current version of this funtion is fairly simple and cannot fully
	/// reverse the operation FlattenPathTo does on a variant with the same params
	/// argument. This primarily has to do with lists.
	void ExpandPathTo( VariantRef expand, const Variant &var, Variant params ) {
		if ( !var.IsMap() ) {
			expand = var;
			return;
		}

		for ( Variant::ConstMapIterator i(var.MapBegin()), e(var.MapEnd()); i != e; ++i ) {
			string path = i->first;
			string delimiter = params["pathDelimiter"].AsString();
			if ( delimiter.length() != 1 ) {
				throw runtime_error("# ExpandPathTo() - Delimiter is not a single char");
			}
			for ( unsigned ii = 0; ii < path.length(); ++ii ) {
				if (path[ii] == delimiter[0]) {
					path[ii] = '/';
				}
			}
			expand.SetPath( path, i->second );
		}
	}

	map< string, string > FlattenPathToStrStrMap ( const Variant &var, Variant params ) {
		map< string, string > flatMap;
		Variant flat = FlattenPath( var, params );

		for ( Variant::MapIterator i(flat.MapBegin()), e(flat.MapEnd()); i != e; ++i ) {
			stringstream tmp;

			switch (i->second.GetType()) {
			case Variant::ListType:
				for ( unsigned j = 0; j < i->second.Size(); ++j ) {
					if (j != 0){
						tmp << params["listDelimiter"].AsString();
					}
					tmp << i->second[j].AsString();
				}
				flatMap[i->first] = tmp.str();
				break;
			case Variant::BlobType:
				throw runtime_error("# FlattenToStrStrMap() - BlobType not supported");
			default:
				flatMap[i->first] = i->second.AsString();
			}
		}

		return flatMap;
	}

	Variant RDiff ( const Variant &base, const Variant &compare ) {
		Variant diff(Variant::MapType);
		RDiff( diff, base, compare );
		return diff;
	}

	void RDiff ( VariantRef diff, const Variant &base, const Variant &compare ) {
		switch ( compare.GetType() ) {
		case Variant::MapType:
			DBPRINT("Start of map" << endl);

			// Replace and break
			if ( !base.IsMap() ) {
				diff = compare.Copy();
				break;
			}

			for  ( Variant::ConstMapIterator i(compare.MapBegin()), e(compare.MapEnd()); i != e; ++i ) {
				DBPRINT(i->first << endl);

				// Replace and continue
				if ( !base.Contains(i->first) ) {
					diff[i->first] = i->second.Copy();
					continue;
				}

				// Recurse into map
				if ( base[i->first] != i->second ) {
					diff[i->first] = Variant::MapType;
					DBPRINT("recurse for " << i->first << endl);
					RDiff( diff[i->first], base[i->first], compare[i->first] );
				}

				// If map diff became null or an empty map, remove - TODO: should this be an option?
				if ( diff.Contains(i->first) ) {
					if ( (diff[ i->first ].GetType() == Variant::NullType) ||
						(diff[ i->first ].GetType() == Variant::MapType && diff[ i->first ].Size() == 0) ) {
							diff.Erase(i->first);
						}
				}
			}
			break;
		default:
			if( base != compare ) {
				diff = compare.Copy();
			}
		}
	}

	void RUpdate( VariantRef base, const Variant &update ) {
		switch ( update.GetType() ) {
		case Variant::NullType:
			return;
		case Variant::MapType:
			if ( !base.Exists() || base.IsMap() ) {
				for ( Variant::ConstMapIterator i(update.MapBegin()), e(update.MapEnd()); i != e; ++i ) {
					RUpdate(base.At( i->first, Variant::MapType ), update[ i->first ]);
				}
				break;
			}
			// else we go to default
		default:
			base = update.Copy();
		}
	}
}
