#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <linux/dvb/dmx.h>
#include <math.h>


#define MAX_SECTION_SIZE 8192
#define MAX_BOUQUETS 16
#define MAX_SECTIONS 256
#define MAX_SERVICES 4096
#define SKY_BOUQUET_ID 25168


#define FILE_LCN_SKY "/tmp/channels.sky"

struct sService
{
  int transport_stream_id;
  int original_network_id;
  int service_id;
  int service_type;
  int stream_type;
  int sky_number;
};

int nServices;
int nServicesLoop;
struct sService *lServices;

struct sSdt
{
  int transport_stream_id;
  int original_network_id;
  int service_id;
  char *service_name;
  char *service_shortname;
  char *service_provider_name;
  char *service_provider_shortname;
};

int nSdts;
int nSdtsLoop;
struct sSdt *lSdts;


struct sNit
{
  int transport_stream_id;
  int original_network_id;
  int service_id;
  int service_type;
  int frequency;
  int orbital_position;
  int west_east_flag;
  int polarization; // 0 = vertical; 1 = horizontal
  int roll_off;
  int modulation_system;
  int modulation_type;
  int symbol_rate;
  int fec_inner;
};

int nNits;
int nNitsLoop;
struct sNit *lNits;

static int bsearch_service_id( const void *A, const void *B )
{
  sService *a = ( sService * ) A;
  sService *b = ( sService * ) B;
  if( a->transport_stream_id > b->transport_stream_id )
  {
    return 1;
  }
  if( a->transport_stream_id < b->transport_stream_id )
  {
    return -1;
  }
  if( a->transport_stream_id == b->transport_stream_id )
  {
    if( a->original_network_id > b->original_network_id )
    {
      return 1;
    }
    if( a->original_network_id < b->original_network_id )
    {
      return -1;
    }
    if( a->original_network_id == b->original_network_id )
    {
      if( a->service_id > b->service_id )
      {
        return 1;
      }
      if( a->service_id < b->service_id )
      {
        return -1;
      }
      if( a->service_id == b->service_id )
      {
        if( a->service_type > b->service_type )
	{
	  return 1;
	}
        if( a->service_type < b->service_type )
	{
	  return -1;
	}
        if( a->service_type == b->service_type )
	{
	  if( a->stream_type > b->stream_type )
	  {
	    return 1;
	  }
	  if( a->stream_type < b->stream_type )
	  {
	    return -1;
	  }
	  if( a->stream_type == b->stream_type )
	  {
	    if( a->sky_number > b->sky_number )
	    {
	      return 1;
	    }
	    if( a->sky_number < b->sky_number )
	    {
	      return -1;
	    }
	  }
	}
      }
    }
  }
  return 0;
}

static int bsearch_sky_number( const void *A, const void *B )
{
  sService *a = ( sService * ) A;
  sService *b = ( sService * ) B;
  if( a->sky_number > b->sky_number )
  {
    return 1;
  }
  if( a->sky_number < b->sky_number )
  {
    return -1;
  }
  if( a->sky_number == b->sky_number )
  {
    if( a->stream_type > b->stream_type )
    {
      return 1;
    }
    if( a->stream_type < b->stream_type )
    {
      return -1;
    }
    if( a->stream_type == b->stream_type )
    {
      if( a->service_type > b->service_type )
      {
        return 1;
      }
      if( a->service_type < b->service_type )
      {
        return -1;
      }
      if( a->service_type == b->service_type )
      {
        if( a->transport_stream_id > b->transport_stream_id )
	{
	  return 1;
	}
        if( a->transport_stream_id < b->transport_stream_id )
	{
	  return -1;
	}
        if( a->transport_stream_id == b->transport_stream_id )
	{
	  if( a->original_network_id > b->original_network_id )
	  {
	    return 1;
	  }
	  if( a->original_network_id < b->original_network_id )
	  {
	    return -1;
	  }
	  if( a->original_network_id == b->original_network_id )
	  {
	    if( a->service_id > b->service_id )
	    {
	      return 1;
	    }
	    if( a->service_id < b->service_id )
	    {
	      return -1;
	    }
	  }
	}
      }
    }
  }
  return 0;
}

static int bsearch_sdt( const void *A, const void *B )
{
  sSdt *a = ( sSdt * ) A;
  sSdt *b = ( sSdt * ) B;
  if( a->transport_stream_id > b->transport_stream_id )
  {
    return 1;
  }
  if( a->transport_stream_id < b->transport_stream_id )
  {
    return -1;
  }
  if( a->transport_stream_id == b->transport_stream_id )
  {
    if( a->original_network_id > b->original_network_id )
    {
      return 1;
    }
    if( a->original_network_id < b->original_network_id )
    {
      return -1;
    }
    if( a->original_network_id == b->original_network_id )
    {
      if( a->service_id > b->service_id )
      {
        return 1;
      }
      if( a->service_id < b->service_id )
      {
        return -1;
      }
    }
  }
  return 0;
}

static int bsearch_nit( const void *A, const void *B )
{
  sService *a = ( sService * ) A;
  sService *b = ( sService * ) B;
  if( a->transport_stream_id > b->transport_stream_id )
  {
    return 1;
  }
  if( a->transport_stream_id < b->transport_stream_id )
  {
    return -1;
  }
  if( a->transport_stream_id == b->transport_stream_id )
  {
    if( a->original_network_id > b->original_network_id )
    {
      return 1;
    }
    if( a->original_network_id < b->original_network_id )
    {
      return -1;
    }
    if( a->original_network_id == b->original_network_id )
    {
      if( a->service_id > b->service_id )
      {
        return 1;
      }
      if( a->service_id < b->service_id )
      {
        return -1;
      }
    }
  }
  return 0;
}

void read_bat( unsigned char *buffer, int length )
{
  int bouquet_id;
  int transport_stream_id;
  int original_network_id;
  int service_id;
  int service_type;
  int stream_type;
  int sky_number;
  int bouquet_descriptors_length;
  int transport_stream_loop_length;
  int transport_descriptors_length;
  int descriptor_tag;
  int descriptor_length;
  int loop, p;
  int loop2, p2;
  int loop3, p3;
  bouquet_id = ( buffer[0] << 8 ) | buffer[1];
  if( bouquet_id == SKY_BOUQUET_ID )
  {
    bouquet_descriptors_length = ( ( buffer[5] & 0x0f ) << 8 ) | buffer[6];
    transport_stream_loop_length = ( ( buffer[bouquet_descriptors_length+7] & 0x0f ) << 8 ) | buffer[bouquet_descriptors_length+8];
    
    // loop
    p = ( bouquet_descriptors_length + 9 );
    loop = transport_stream_loop_length;
    while( loop > 0 )
    {
      transport_stream_id = ( buffer[p+0] << 8 ) | buffer[p+1];
      original_network_id = ( buffer[p+2] << 8 ) | buffer[p+3];
      transport_descriptors_length = ( ( buffer[p+4] & 0x0f ) << 8 ) | buffer[p+5];
      p2 = ( p + 6 );
      loop2 = transport_descriptors_length;
      while( loop2 > 0 )
      {
        descriptor_tag = buffer[p2+0];
	descriptor_length = buffer[p2+1];
	switch( descriptor_tag )
	{
	  case 0x41: // service list descriptor
	    /*
	    p3 = ( p2 + 2 );
	    loop3 = descriptor_length;
	    while( loop3 > 0 )
	    {
	      service_id = ( buffer[p3+0] << 8 ) | buffer[p3+1];
	      service_type = buffer[p3+2];
	      sService sk, *ss;
	      sk.transport_stream_id = transport_stream_id;
	      sk.original_network_id = original_network_id;
	      sk.service_id = service_id;
	      ss = ( sService * ) bsearch( &sk, lServices, nServices, sizeof( sService ), &bsearch_service_id );
	      if( ss == NULL )
	      {
	        ss = ( lServices + nServices );
		ss->transport_stream_id = transport_stream_id;
		ss->original_network_id = original_network_id;
		ss->service_id = service_id;
		ss->service_type = service_type;
		nServices ++;
	        qsort( lServices, nServices, sizeof( sService ), &bsearch_service_id );
	      }
	      p3 += 3;
	      loop3 -= 3;
	    }
	    */
	    break;
	  case 0x5f:
	    break;
	  case 0xb1:
	    p3 = ( p2 + 2 );
	    stream_type = ( buffer[p3+0] << 8 ) | buffer[p3+1];
	    p3 = ( p3 + 2 );
	    loop3 = descriptor_length;
	    switch( stream_type )
	    {
	      case 0: // ricevitori sky sd
	      case 2: // ricevitori sky hd
	      case 65535: // tutti i ricevitori
	        while( loop3 > 0 )
		{
		  service_id = ( buffer[p3+0] << 8 ) | buffer[p3+1];
		  service_type = buffer[p3+2];
		  sky_number = ( buffer[p3+5] << 8 ) | buffer[p3+6];
		  switch( service_type )
		  {
		    case 0x01:
		    case 0x02:
		    case 0x04:
		    case 0x05:
		    case 0x16:
		    case 0x17:
		    case 0x18:
		    case 0x19:
		    case 0x1a:
		    case 0x1b:
		      sService sk, *ss;
		      sk.transport_stream_id = transport_stream_id;
		      sk.original_network_id = original_network_id;
		      sk.service_id = service_id;
		      sk.service_type = service_type;
		      sk.stream_type = stream_type;
		      sk.sky_number = sky_number;
		      ss = ( sService * ) bsearch( &sk, lServices, nServices, sizeof( sService ), &bsearch_service_id );
		      if( ss == NULL )
		      {
		        ss = ( lServices + nServices );
			ss->transport_stream_id = transport_stream_id;
			ss->original_network_id = original_network_id;
			ss->service_id = service_id;
			ss->service_type = service_type;
			ss->stream_type = stream_type;
			ss->sky_number = sky_number;
			nServices ++;
			qsort( lServices, nServices, sizeof( sService ), &bsearch_service_id );
		      }
		      else
		      {
		        nServicesLoop ++;
		      }
		      break;
		    default:
		      break;
		  }
		  p3 += 9;
		  loop3 -= 9;
		}
	        break;
	      default:
	        break;
	    }
	    break;
	  default:
	    fprintf( stderr, "BAT: non trovato tag 0x%02x\n", descriptor_tag );
	    break;
	}
        p2 += ( descriptor_length + 2 );
	loop2 -= ( descriptor_length + 2 );
      }
      p += ( transport_descriptors_length + 6 );
      loop -= ( transport_descriptors_length + 6 );
    }
  }
}

void read_sdt( unsigned char *buffer, int length )
{
  int transport_stream_id;
  int original_network_id;
  int service_id;
  int service_type;
  int service_provider_name_length;
  int service_name_length;
  int descriptors_loop_length;
  int descriptor_tag;
  int descriptor_length;
  int loop, p;
  int loop1, p1;
  int p2;
  int full_length;
  int short_length;
  int emphasis_on;
  transport_stream_id = ( buffer[0] << 8 ) | buffer[1];
  original_network_id = ( buffer[5] << 8 ) | buffer[6];
  p = 8;
  loop = ( length - 8 );
  while( loop > 0 )
  {
    service_id = ( buffer[p+0] << 8 ) | buffer[p+1];
    descriptors_loop_length = ( ( buffer[p+3] & 0x0f ) << 8 ) | buffer[p+4];
    p1 = ( p + 5 );
    loop1 = descriptors_loop_length;
    while( loop1 > 0 )
    {
      descriptor_tag = buffer[p1+0];
      descriptor_length = buffer[p1+1];
      switch( descriptor_tag )
      {
        case 0x48: // Service Descriptor
	  sSdt sk, *ss;
	  sk.transport_stream_id = transport_stream_id;
	  sk.original_network_id = original_network_id;
	  sk.service_id = service_id;
	  ss = ( sSdt * ) bsearch( &sk, lSdts, nSdts, sizeof( sSdt ), &bsearch_sdt );
	  if( ss == NULL )
	  {
	    ss = ( lSdts + nSdts );
	    ss->transport_stream_id = transport_stream_id;
	    ss->original_network_id =  original_network_id;
	    ss->service_id = service_id;
	    service_type = buffer[p1+2];
	    
	    service_provider_name_length = buffer[p1+3];
	    service_name_length = buffer[p1+service_provider_name_length+4];
	    
	    ss->service_provider_name = (char *) calloc((service_provider_name_length + 1), sizeof(char));
	    ss->service_provider_shortname = (char *) calloc((service_provider_name_length + 1), sizeof(char));
	    ss->service_name = (char *) calloc((service_name_length + 1), sizeof(char));
	    ss->service_shortname = (char *) calloc((service_name_length + 1), sizeof(char));
	    
	    p2 = (p1 + 4);
	    full_length = 0;
	    short_length = 0;
	    emphasis_on = 0;
	    for(int i = 0; i < service_provider_name_length; i++)
	    {
	      switch(buffer[p2+i])
	      {
	        case 0x01 ... 0x1f:
		case 0x80 ... 0x85:
		case 0x88 ... 0x89:
		case 0x8a ... 0x9f:
		  break;
		case 0x86:
		  emphasis_on = 1;
		  break;
		case 0x87:
		  emphasis_on = 0;
		  break;
		default:
		  if(emphasis_on == 1)
		  {
		    ss->service_provider_shortname[short_length++] = buffer[p2+i];
		  }
		  ss->service_provider_name[full_length++] = buffer[p2+i];
		  break;
	      }
	    }
	    if(!ss->service_provider_name[0])
	    {
	      free(ss->service_provider_name);
	      ss->service_provider_name = 0;
	    }
	    if(!ss->service_provider_shortname[0])
	    {
	      free(ss->service_provider_shortname);
	      ss->service_provider_shortname = 0;
	    }
	    
	    p2 = (p1 + service_provider_name_length + 5);
	    full_length = 0;
	    short_length = 0;
	    emphasis_on = 0;
	    for(int i = 0; i < service_name_length; i++)
	    {
	      switch(buffer[p2+i])
	      {
	        case 0x01 ... 0x1f:
		case 0x80 ... 0x85:
		case 0x88 ... 0x89:
		case 0x8a ... 0x9f:
		  break;
		case 0x86:
		  emphasis_on = 1;
		  break;
		case 0x87:
		  emphasis_on = 0;
		  break;
		default:
		  if(emphasis_on == 1)
		  {
		    ss->service_shortname[short_length++] = buffer[p2+i];
		  }
		  ss->service_name[full_length++] = buffer[p2+i];
		  break;
	      }
	    }
	    if(!ss->service_name[0])
	    {
	      free(ss->service_name);
	      ss->service_name = 0;
	    }
	    if(!ss->service_shortname[0])
	    {
	      free(ss->service_shortname);
	      ss->service_shortname = 0;
	    }
	    nSdts ++;
	    qsort( lSdts, nSdts, sizeof( sSdt ), &bsearch_sdt );
	  }
	  else
	  {
	    nSdtsLoop ++;
	  }
	  break;
	case 0x49:
	  
	  break;
	case 0x4a: // Linkage descriptor
	  
	  break;
	case 0x4b: // Near Video On Demand (NVOD) reference descriptor
	  
	  break;
	case 0x4c: // Time shifted service descriptor
	  
	  break;
	case 0x5f:
	  
	  break;
	case 0xb2:
	  
	  break;
	case 0xc0:
	  
	  break;
        default:
	  fprintf( stderr, "SDT: non trovato tag %02x\n", descriptor_tag );
	  break;
      }
      p1 += ( descriptor_length + 2 );
      loop1 -= ( descriptor_length + 2 );
    }
    p += ( descriptors_loop_length + 5 );
    loop -= ( descriptors_loop_length + 5 );
  }
}

int round_value( double value )
{
  double integral_part;
  double fractional_part;
  fractional_part = modf( value, &integral_part );
  if( fractional_part >= .5 )
  {
    return (int) (integral_part + 1);
  }
  else
  {
    return (int) integral_part;
  }
}

void read_nit( unsigned char *buffer, int length )
{
  int network_id;
  int network_descriptors_length;
  int transport_stream_loop_length;
  int transport_descriptors_length;
  int transport_stream_id;
  int original_network_id;
  int descriptor_tag;
  int descriptor_length;
  int service_id;
  int service_type;
  int service_id_min;
  int n_services;
  int p1, loop1;
  network_id = ( buffer[0] << 8 ) | buffer[1];
  network_descriptors_length = ( ( buffer[5] & 0x0f ) << 8 ) | buffer[6];
  buffer += 7;
  while( network_descriptors_length > 0 )
  {
    descriptor_tag = buffer[0];
    descriptor_length = buffer[1];
    switch(descriptor_tag)
    {
      case 0x40:
        
        break;
      case 0x4a:
        
        break;
      case 0x5f:
        
        break;
      default:
        fprintf( stderr, "NIT DESCRIPTOR: non trovato tag %02x\n", descriptor_tag );
        break;
    }
    network_descriptors_length -= ( descriptor_length + 2 );
    buffer += ( descriptor_length + 2 );
  }
  transport_stream_loop_length = ( ( buffer[0] & 0x0f ) << 8 ) | buffer[1];
  buffer += 2;
  while( transport_stream_loop_length > 0 )
  {
    transport_stream_id = ( buffer[0] << 8 ) | buffer[1];
    original_network_id = ( buffer[2] << 8 ) | buffer[3];
    service_id_min = 0xffff;
    n_services = 0;
    transport_descriptors_length = ( ( buffer[4] & 0x0f ) << 8 ) | buffer[5];
    transport_stream_loop_length -= ( transport_descriptors_length + 6 );
    buffer += 6;
    while( transport_descriptors_length > 0 )
    {
      descriptor_tag = buffer[0];
      descriptor_length = buffer[1];
      switch(descriptor_tag)
      {
        case 0x41: // service_list_descriptor
          p1 = 2;
          loop1 = descriptor_length;
          while( loop1 > 0 )
          {
            service_id = ( buffer[p1+0] << 8 ) | buffer[p1+1];
            service_type = buffer[p1+2];
            if( service_id > 0 )
            {
              switch(service_type)
              {
                case 0x01:
                case 0x02:
                case 0x04:
                case 0x05:
                case 0x16:
                case 0x17:
                case 0x18:
                case 0x19:
                case 0x1a:
                case 0x1b:
                  sNit sk, *ss;
                  sk.transport_stream_id = transport_stream_id;
                  sk.original_network_id = original_network_id;
                  sk.service_id = service_id;
                  ss = ( sNit * ) bsearch( &sk, lNits, nNits, sizeof( sNit ), &bsearch_nit );
                  if( ss == NULL )
                  {
                    ss = ( lNits + nNits );
                    ss->transport_stream_id = transport_stream_id;
                    ss->original_network_id = original_network_id;
                    ss->service_id = service_id;
                    ss->service_type = service_type;
                    nNits ++;
                    qsort( lNits, nNits, sizeof( sNit ), &bsearch_nit );
                    if( service_id < service_id_min )
                    {
                      service_id_min = service_id;
                    }
                    n_services ++;
                  }
                  else
                  {
                    nNitsLoop ++;
                  }
                  break;
                default:
                  break;
              }
            }
            p1 += 3;
            loop1 -= 3;
          }
          break;
        case 0x43: // satellite_delivery_system_descriptor
          sNit sk, *ss;
          sk.transport_stream_id = transport_stream_id;
          sk.original_network_id = original_network_id;
          sk.service_id = service_id_min;
          ss = ( sNit * ) bsearch( &sk, lNits, nNits, sizeof( sNit ), &bsearch_nit );
          if( ss )
          {
            double frequency = (((buffer[2]&0xf0)>>4)*10000000) +
                                   (((buffer[2]&0x0f))*1000000) +
                                 (((buffer[3]&0xf0)>>4)*100000) +
                                     (((buffer[3]&0x0f))*10000) +
                                   (((buffer[4]&0xf0)>>4)*1000) +
                                       (((buffer[4]&0x0f))*100) +
                                     (((buffer[5]&0xf0)>>4)*10) +
                                             (buffer[5]&0x0f*1);
            
            int orbital_position = (((buffer[6]&0xf0)>>4)*1000) +
                                       (((buffer[6]&0x0f))*100) +
                                     (((buffer[7]&0xf0)>>4)*10) + 
                                             (buffer[7]&0x0f*1);
            
            double symbol_rate = (((buffer[9]&0xf0)>>4)*1000000) +
                                       ((buffer[9]&0x0f)*100000) +
                                  (((buffer[10]&0xf0)>>4)*10000) +
                                        ((buffer[10]&0x0f)*1000) +
                                    (((buffer[11]&0xf0)>>4)*100) +
                                          ((buffer[11]&0x0f)*10) +
                                      (((buffer[12]&0xf0)>>4)*1);
            
            while( n_services > 0 )
            {
              if( ss->transport_stream_id == transport_stream_id )
              {
                if( ss->original_network_id == original_network_id )
                {
                  ss->frequency = round_value(frequency/100);
                  ss->orbital_position = orbital_position;
                  ss->west_east_flag = ( buffer[8] & 0x80 ) >> 7;
                  ss->polarization = ( buffer[8] & 0x60 ) >> 5;
                  ss->modulation_system = ( buffer[8] & 0x04 ) >> 2;
                  ss->modulation_type = buffer[8] & 0x03;
                  if( ss->modulation_system == 1 )
                  {
                    ss->roll_off = ( buffer[8] & 0x18 ) >> 3;
                  }
                  else
                  {
                    ss->roll_off = 0;
                  }
                  ss->symbol_rate = round_value(symbol_rate/10);
                  ss->fec_inner = buffer[12] & 0x0f;
                }
              }
              n_services --;
              ss ++;
            }
          }
          break;
        default:
          fprintf( stderr, "NIT: non trovato tag %02x\n", descriptor_tag );
          break;
      }
      transport_descriptors_length -= ( descriptor_length + 2 );
      buffer += ( descriptor_length + 2 );
    }
  }
  
  nNitsLoop ++;
}

int main()
{
  int demux_fd;
  const char *demux_device = "/dev/dvb/adapter0/demux0";
  struct dmx_sct_filter_params filter;
  unsigned char demux_mask[16];
  unsigned char buffer[MAX_SECTION_SIZE];
  int bytes;
  int length;
  FILE *fd;
  int stepFilter;
  
  stepFilter = 0;
  
  nServices = 0;
  nServicesLoop = 0;
  lServices = ( struct sService * ) calloc( MAX_SERVICES, sizeof( struct sService ) );
  
  nSdts = 0;
  nSdtsLoop = 0;
  lSdts = ( struct sSdt * ) calloc( MAX_SERVICES, sizeof( struct sSdt ) );
  
  nNits = 0;
  nNitsLoop = 0;
  lNits = ( struct sNit * ) calloc( MAX_SERVICES, sizeof( struct sNit ) );
  
  // leggo la BAT e SDT e NIT
  setDemux:;
  if( ( demux_fd = open( demux_device, O_RDWR ) ) < 0 )
  {
    perror( "open demux device: " );
    return -1;
  }
  memset( demux_mask, 0, sizeof( demux_mask ) );
  memset( &filter.filter, 0, sizeof( struct dmx_filter ) );
  if( stepFilter == 2 )
  {
    filter.pid = 0x10; // NIT
    stepFilter = 3;
  }
  else
  {
    filter.pid = 0x11; // BAT e SDT
  }
  memcpy( filter.filter.filter, demux_mask, DMX_FILTER_SIZE );
  memcpy( filter.filter.mask, demux_mask, DMX_FILTER_SIZE );
  filter.timeout = 0;
  filter.flags = DMX_IMMEDIATE_START | DMX_CHECK_CRC;
  if( ioctl( demux_fd, DMX_SET_FILTER, &filter ) == -1 )
  {
    perror( "set demux filter: " );
    close( demux_fd );
    return -1;
  }
  
  while( 1 )
  {
    bytes = read( demux_fd, buffer, sizeof( buffer ) );
    
    if( bytes < 0 )
    {
      perror( "read demux: " );
      if( errno != EOVERFLOW )
      {
        close( demux_fd );
	return -1;
      }
    }
    
    length = ( ( ( buffer[1] & 0x0f ) << 8 ) | buffer[2] ) - 7;
    
    // BAT
    if( filter.pid == 0x11 && buffer[0] == 0x4a )
    {
      if( stepFilter == 0 )
      {
        read_bat( &buffer[3], length );
        if( ( nServicesLoop * 4 ) > nServices )
        {
          fprintf( stderr, "Trovati %i servizi nella BAT\n", nServices );
	  stepFilter = 1;
        }
      }
    }
    
    // SDT
    if( filter.pid == 0x11 && ( buffer[0] == 0x42 || buffer[0] == 0x46 ) )
    {
      if( stepFilter == 1 )
      {
        read_sdt( &buffer[3], length );
        if( ( nSdtsLoop * 4 ) > nSdts )
        {
          fprintf( stderr, "Trovati %i servizi nella SDT\n", nSdts );
	  stepFilter = 2;
        }
      }
    }
    
    // NIT
    if( filter.pid == 0x10 && ( buffer[0] == 0x40 || buffer[0] == 0x41 ) )
    {
      if( stepFilter == 3 )
      {
        read_nit( &buffer[3], length );
        if( ( nNitsLoop * 4 ) > nNits )
        {
          fprintf( stderr, "Trovati %i servizi nella NIT\n", nNits );
          stepFilter = 4;
        }
      }
    }
    
    if( stepFilter == 2 || stepFilter == 4 )
    {
      break;
    }
  }
  
  ioctl( demux_fd, DMX_STOP);
  close( demux_fd );
  if( stepFilter == 2 )
  {
    goto setDemux;
  }
  
  // eseguo il riordino
  qsort( lServices, nServices, sizeof( sService ), &bsearch_sky_number );
  qsort( lSdts, nSdts, sizeof( sSdt ), &bsearch_sdt );
  qsort( lNits, nNits, sizeof( sNit ), &bsearch_nit );
  
  // salvo i canali trovati
  fd = fopen( FILE_LCN_SKY, "w" );
  if( fd == NULL )
  {
    perror( "file open: " );
    return -1;
  }
  
  for( int i = 0; i < nServices; i ++ )
  {
    char *service_provider_name = 0;
    char *service_provider_shortname = 0;
    char *service_name = 0;
    char *service_shortname = 0;
    int frequency = 999;
    int orbital_position = 999;
    int west_east_flag = 999;
    int polarization = 999;
    int roll_off = 999;
    int modulation_system = 999;
    int modulation_type = 999;
    int symbol_rate = 999;
    int fec_inner = 999;
    
    sService *service = ( lServices + i );
    for( int ii = 0; ii < nSdts; ii ++ )
    {
      struct sSdt *sdt = ( lSdts + ii );
      if( sdt->transport_stream_id == service->transport_stream_id )
      {
        if( sdt->original_network_id == service->original_network_id )
	{
	  if( sdt->service_id == service->service_id )
	  {
	    service_provider_name = sdt->service_provider_name;
	    service_provider_shortname = sdt->service_provider_shortname;
	    service_name = sdt->service_name;
	    service_shortname = sdt->service_shortname;
	    break;
	  }
	}
      }
    }
    
    for( int ii = 0; ii < nNits; ii ++ )
    {
      struct sNit *nit = ( lNits + ii );
      if( nit->transport_stream_id == service->transport_stream_id )
      {
        if( nit->original_network_id == service->original_network_id )
        {
          if( nit->service_id == service->service_id )
          {
            frequency = nit->frequency;
            orbital_position = nit->orbital_position;
            west_east_flag = nit->west_east_flag;
            polarization = nit->polarization;
            roll_off = nit->roll_off;
            modulation_system = nit->modulation_system;
            modulation_type = nit->modulation_type;
            symbol_rate = nit->symbol_rate;
            fec_inner = nit->fec_inner;
          }
        }
      }
    }
    
    fprintf( fd, 
      "%i|%i|%i|%i|%i|%i|%i|%i|%i|%i|%i|%i|%i|%i|%i|%s|%s\n",
      service->sky_number,
      service->stream_type,
      service->service_type,
      service->transport_stream_id,
      service->original_network_id,
      service->service_id,
      
      frequency,
      orbital_position,
      west_east_flag,
      polarization,
      roll_off,
      modulation_system,
      modulation_type,
      symbol_rate,
      fec_inner,
      
      service_name,
      service_provider_name
    );
  }
  fclose( fd );
  
  free( lSdts );
  free( lServices );
  free( lNits );
  return 0;
}
