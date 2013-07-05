#pragma once

#include "linearOperator.h"
#include "gpupmri_export.h"

#include <boost/smart_ptr.hpp>
#include <iostream>

template<class REAL, unsigned int D, class ARRAY_TYPE> class EXPORTGPUPMRI senseOperator 
	: public linearOperator<REAL, ARRAY_TYPE>
{

public:

  senseOperator() : linearOperator<REAL, ARRAY_TYPE>(), ncoils_(0) {}
  virtual ~senseOperator() {}

  inline unsigned int get_number_of_coils() { return ncoils_; }
  inline boost::shared_ptr<ARRAY_TYPE> get_csm() { return csm_; }

  virtual int set_csm( boost::shared_ptr<ARRAY_TYPE> csm )
  {
    if( csm.get() && csm->get_number_of_dimensions() == D+1 ) {
      csm_ = csm;      
      ncoils_ = csm_->get_size(D);
      std::vector<unsigned int> tmp_dims = *csm_->get_dimensions();
      tmp_dims.pop_back();
      this->set_domain_dimensions(&tmp_dims);
      return 0;
    }
    else{
      std::cerr << "Error: senseOperator::set_csm : unexpected csm dimensionality" << std::endl;
      return -1;
    }    
  }

  virtual int mult_M( ARRAY_TYPE* in, ARRAY_TYPE* out, bool accumulate = false ) = 0;
  virtual int mult_MH( ARRAY_TYPE* in, ARRAY_TYPE* out, bool accumulate = false ) = 0;

  virtual int mult_csm( ARRAY_TYPE* in, ARRAY_TYPE* out ) = 0;
  virtual int mult_csm_conj_sum( ARRAY_TYPE* in, ARRAY_TYPE* out) = 0;

protected:

  unsigned int ncoils_;
  boost::shared_ptr< ARRAY_TYPE > csm_;
};