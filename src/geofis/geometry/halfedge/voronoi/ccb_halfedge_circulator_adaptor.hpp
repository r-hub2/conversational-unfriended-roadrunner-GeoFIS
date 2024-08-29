/**
 * GeoFIS R package
 *
 * Copyright (C) 2021 INRAE
 *
 * Authors:
 * 	Jean-luc Lablée - INRAE
 * 	Serge Guillaume - INRAE
 *
 * License: CeCILL v2.1
 * 	https://cecill.info/licences/Licence_CeCILL_V2.1-en.html
 * 	https://cecill.info/licences/Licence_CeCILL_V2.1-fr.html
 *
 * This software is governed by the CeCILL license under French law and
 * abiding by the rules of distribution of free software.  You can  use,
 * modify and/ or redistribute the software under the terms of the CeCILL
 * license as circulated by CEA, CNRS and INRIA at the following URL
 * "https://cecill.info".
 *
 * As a counterpart to the access to the source code and  rights to copy,
 * modify and redistribute granted by the license, users are provided only
 * with a limited warranty  and the software's author,  the holder of the
 * economic rights, and the successive licensors have only limited liability.
 *
 * In this respect, the user's attention is drawn to the risks associated
 * with loading,  using,  modifying and/or developing or reproducing the
 * software by the user in light of its specific status of free software,
 * that may mean  that it is complicated to manipulate,  and  that  also
 * therefore means  that it is reserved for developers  and  experienced
 * professionals having in-depth computer knowledge. Users are therefore
 * encouraged to load and test the software's suitability as regards their
 * requirements in conditions enabling the security of their systems and/or
 * data to be ensured and,  more generally, to use and operate it in the
 * same conditions as regards security.
 *
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL license and that you accept its terms.
 */
#ifndef CCB_HALFEDGE_CIRCULATOR_ADAPTOR_NEVER_EMPTY_HPP_
#define CCB_HALFEDGE_CIRCULATOR_ADAPTOR_NEVER_EMPTY_HPP_

#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <CGAL/Voronoi_diagram_2/Circulator_adaptors.h>
#include <CGAL/Voronoi_diagram_2/Halfedge.h>

namespace CGAL {

namespace VoronoiDiagram_2 { namespace Internal {

// Ccb_halfedge_circulator_adaptor specialization for Halfedge<VDA>
// the default Ccb_halfedge_circulator_adaptor impementation do not provide const_reference_type operator*() const
template <class VDA> class Ccb_halfedge_circulator_adaptor<Halfedge<VDA> > : public Circulator_from_halfedge_adaptor<Halfedge<VDA>, Ccb_halfedge_circulator_adaptor<Halfedge<VDA> > > {

	typedef Halfedge<VDA> 												arg;
	typedef Ccb_halfedge_circulator_adaptor<arg> 						self;
	typedef Circulator_from_halfedge_adaptor<arg, self> 				base;
	typedef typename base::reference 									reference_type;
	typedef typename boost::add_const<typename base::value_type>::type 	const_value_type;
	typedef typename boost::add_reference<const_value_type>::type 		const_reference_type;

	friend class Circulator_from_halfedge_adaptor<arg, self>;

public:

	Ccb_halfedge_circulator_adaptor() : base() {}
	Ccb_halfedge_circulator_adaptor(const arg &he) : base(he) {}

	reference_type operator*() { return this->cur_; }
	const_reference_type operator*() const { return this->cur_; }

private:
	Ccb_halfedge_circulator_adaptor(const base &b) : base(b) {}

	void increment() {
		this->cur_ = *this->cur_.next();
	}

	void decrement() {
		this->cur_ = *this->cur_.previous();
	}
};

}} // namespace VoronoiDiagram_2::Internal

// for use macro CGAL_For_all with voronoi circulator we must consider that Ccb_halfedge_circulator_adaptor is never empty
template<class Halfedge> inline bool operator==(const VoronoiDiagram_2::Internal::Ccb_halfedge_circulator_adaptor<Halfedge> &, const Nullptr_t) {
	return false;
}

template<class Halfedge> inline bool operator!=(const VoronoiDiagram_2::Internal::Ccb_halfedge_circulator_adaptor<Halfedge> &circulator, const Nullptr_t ptr) {
	return !(circulator == ptr);
}

} // namespace CGAL

#endif /* CCB_HALFEDGE_CIRCULATOR_ADAPTOR_NEVER_EMPTY_HPP_ */
