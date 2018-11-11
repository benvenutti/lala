/*
 * Copyright (c) 2014 Rafael Vega <rvega@elsoftwarehamuerto.org>
 *
 * BSD Simplified License.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 *
 * See https://github.com/libpd/libpd for documentation
 *
 */
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <vector>

#include "PdBase.hpp"
#include <jack/jack.h>

#include "PdObject.h"

#include <boost/range/adaptors.hpp>
#include <boost/range/irange.hpp>

int          sampleRate = 0;
int          ticks      = 0;
unsigned int i;

jack_client_t* client;
jack_port_t*   portO1;
jack_port_t*   portO2;

pd::PdBase lpd;
PdObject   pdObject;

float*                       output = (float*) malloc( 1024 * 2 * sizeof( float ) );
jack_default_audio_sample_t* out1;
jack_default_audio_sample_t* out2;

int process( jack_nframes_t nframes, void* arg )
{

    // get pointers to the output buffers
    out1 = (jack_default_audio_sample_t*) jack_port_get_buffer( portO1, nframes );
    out2 = (jack_default_audio_sample_t*) jack_port_get_buffer( portO2, nframes );

    // pass audio samples to/from libpd
    ticks = nframes / 64;
    lpd.processFloat( ticks, nullptr, output );

    // Jack uses mono ports and pd expects interleaved stereo buffers.
    for ( const auto i : boost::irange( jack_nframes_t{ 0 }, nframes ) )
    {
        *out1 = output[i * 2];
        *out2 = output[( i * 2 ) + 1];
        out1++;
        out2++;
    }

    return 0;
}

void init()
{

    // create client
    if ( ( client = jack_client_open( "LibPdTest", JackNullOption, NULL ) ) == NULL )
    {
        std::cerr << "Jack server not running?" << std::endl;
        exit( 1 );
    }

    // get sample rate from jack server
    sampleRate = jack_get_sample_rate( client );

    // init pd
    if ( !lpd.init( 0, 2, sampleRate ) )
    {
        std::cerr << "Could not init pd" << std::endl;
        exit( 1 );
    }

    // receive messages from pd
    lpd.setReceiver( &pdObject );
    lpd.subscribe( "cursor" );

    // send DSP 1 message to pd
    lpd.computeAudio( true );

    // load the patch
    pd::Patch patch = lpd.openPatch( "test.pd", "./pd" );
    std::cout << patch << std::endl;

    // register io ports
    portO1 = jack_port_register( client, "out1", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0 );
    portO2 = jack_port_register( client, "out2", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0 );

    // register audio process callback
    jack_set_process_callback( client, process, 0 );

    // go!
    if ( jack_activate( client ) )
    {
        std::cout << "Could not activate client";
        exit( 1 );
    }
}

int main()
{
    const std::vector< int > nums{ 1, 2, 3, 4, 5, 6, 7, 8 };

    for ( const auto n : nums | boost::adaptors::filtered( []( auto i ) { return i % 2 == 0; } ) )
    {
        std::cout << n << std::endl;
    }

    init();

    // keep the program alive until it's killed with Ctrl+C
    while ( true )
    {
        lpd.receiveMessages();
        lpd.sendFloat( "FromCpp", 578 );
        usleep( 100 );
    }

    return EXIT_SUCCESS;
}
