#ifndef EVKW_RINGBUFFER_PRIVATE_H
#define EVKW_RINGBUFFER_PRIVATE_H

/*---------------------------------------------------------------------------*/
#define ringb_pos_private(ringb_ptr, position_)\
  (((ringb_ptr)->end - (ringb_ptr)->size + (position_)) &\
    (ringb_ptr)->mask)
/*---------------------------------------------------------------------------*/
#define ringb_end_add_private(ringb_ptr, positions_)\
  (((ringb_ptr)->end + (positions_)) & (ringb_ptr)->mask)
/*---------------------------------------------------------------------------*/
#define ringb_end_sub_private(ringb_ptr, positions_)\
  (((ringb_ptr)->end - (positions_)) & (ringb_ptr)->mask)
/*---------------------------------------------------------------------------*/
#define ringb_typebuf_cast_private(ringb_ptr, raw_position_, val_type)\
  ((val_type*) &(ringb_ptr)->type_buff[(raw_position_) * sizeof (val_type)])
/*---------------------------------------------------------------------------*/

#endif
