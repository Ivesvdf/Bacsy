/*
 * Copyright (C) 2011  Ives van der Flaas
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <sstream>
#include <fstream>
#include <gtest/gtest.h>
#include "Bacsy/Streams/ToSignatureStream.h"
#include "Bacsy/Streams/LoadSignatureStream.h"
#include "Bacsy/Streams/SimpleBufferStream.h"
#include "Bacsy/Streams/PerformDeltaStream.h"

namespace Bacsy
{

TEST( SignatureStreamTest, HelloWorldTest )
{
	SimpleBufferStream outputStream;

	{
		ToSignatureStream stream(outputStream, 8);
		stream.write("hello world!", 12);
	}

	char helloWorldData[] = 
	{0x72,0x73,0x01,0x36,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x08,0x27,0xEF,0x05
,0xF1,0x33,0x63,0xB7,0x28,0x40,0xAC,0xD5,0xF4};

	unsigned int n;
	char *actual = outputStream.getBuffer(&n);

	ASSERT_EQ(sizeof(helloWorldData), n);

	for(unsigned int i = 0; i < n; i++)
	{
		ASSERT_EQ(helloWorldData[i], actual[i]);
	}
}

TEST( SignatureStreamTest, LongerTest )
{
	char longer[] = 
		"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed quis justo sapien. Morbi gravida fringilla risus, quis tempus mauris tempus id. Maecenas gravida, nisi at rhoncus dictum, nisl lorem mattis mauris, sed dignissim purus augue eu dolor. Suspendisse blandit ligula suscipit neque varius porta. Nullam id lorem ut lorem vehicula varius. Quisque sed lacus nunc. Donec porta facilisis risus, sed ornare augue sollicitudin sed. Sed a nibh non metus adipiscing gravida. Etiam velit orci, pulvinar id fermentum nec, faucibus et mauris. Etiam in lacinia libero. Aliquam erat volutpat. Etiam lacus ante, euismod in lacinia eget, mollis a erat. Vestibulum at rhoncus odio. Morbi sed ligula in lacus placerat mollis. Nullam nec odio vel elit ultricies varius et et dolor. Integer id ante lacus. Duis dictum, massa sed bibendum consequat, nibh enim iaculis nunc, vitae volutpat lorem tortor ut lorem.\n"
"\n"
"Proin vel mi lorem, vitae aliquet eros. Integer dapibus est gravida eros adipiscing pulvinar. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Pellentesque faucibus posuere lorem at porttitor. Phasellus consectetur porta ligula et fringilla. Aliquam arcu quam, posuere sit amet bibendum sit amet, consequat et risus. Cras blandit egestas libero. Aenean consequat placerat felis, faucibus tincidunt erat tristique et. Mauris mauris ligula, tempor vel porta sit amet, dictum sit amet eros. Ut magna velit, vulputate a vehicula non, consectetur sit amet libero.\n"
"\n"
"Vestibulum auctor lectus ac odio auctor in ultrices leo consectetur. Aliquam elementum laoreet libero eget tempus. Aenean eget ullamcorper libero. Aenean vulputate libero eros, non commodo sem. Aliquam sit amet nibh mi. Nam hendrerit tincidunt scelerisque. Quisque condimentum bibendum malesuada. Maecenas nibh lorem, commodo quis tempus rhoncus, accumsan eget eros. Aliquam suscipit, est eu venenatis rhoncus, elit enim porta est, eget semper lorem metus id arcu. Fusce feugiat iaculis nulla eget sagittis. Aliquam feugiat feugiat metus sit amet ornare. Vivamus arcu lectus, lacinia consectetur molestie vel, malesuada vitae est. Proin vitae dui sem. Nulla congue augue sollicitudin dui pretium a volutpat ligula ultricies. Sed tortor purus, tincidunt vel consectetur ac, sollicitudin vitae odio. Nunc a commodo tortor.\n"
"\n"
"Nam mollis nibh sed ante pulvinar ultrices. Curabitur vel est lorem. Suspendisse potenti. Pellentesque in augue diam, eget luctus justo. Praesent et odio neque. Integer mattis suscipit porttitor. Fusce tempor gravida quam, sit amet ultricies nunc lacinia ac. Sed non mi sit amet tellus sollicitudin rhoncus. Nulla facilisi. Phasellus ut tellus a magna porta rhoncus. Vivamus consequat consequat nisi, quis tempus urna feugiat ac. Praesent eu dolor eros, in pulvinar enim. Etiam in sapien sed enim vestibulum bibendum sit amet at arcu. In hac habitasse platea dictumst. Ut accumsan, nisl a fringilla ultricies, lacus felis hendrerit tortor, nec tincidunt ligula neque in est. Nunc eget sed.\n";

	char sig[] = 
	{0x72,0x73,0x01,0x36,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x08,0x1F,0x45,0xE7
	,0xAB,0x32,0xB9,0x02,0x40,0x53,0xF6,0x62,0x6B,0x48,0x75,0xD3,0x07,0xB3,0xF1
	,0xF9,0x5B,0xE0,0xC7,0x3C,0x17};


	SimpleBufferStream outputStream;
	{
		ToSignatureStream stream(outputStream, 8);
		stream.write(longer, sizeof(longer)-1);
	}

	unsigned int n;
	char *actual = outputStream.getBuffer(&n);

	ASSERT_EQ(sizeof(sig), n);

	for(unsigned int i = 0; i < n; i++)
	{
		ASSERT_EQ(sig[i], actual[i]);
	}
}


TEST( SignatureStreamTest, SimulateFileTest )
{
	char longer[] = 
		"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed quis justo sapien. Morbi gravida fringilla risus, quis tempus mauris tempus id. Maecenas gravida, nisi at rhoncus dictum, nisl lorem mattis mauris, sed dignissim purus augue eu dolor. Suspendisse blandit ligula suscipit neque varius porta. Nullam id lorem ut lorem vehicula varius. Quisque sed lacus nunc. Donec porta facilisis risus, sed ornare augue sollicitudin sed. Sed a nibh non metus adipiscing gravida. Etiam velit orci, pulvinar id fermentum nec, faucibus et mauris. Etiam in lacinia libero. Aliquam erat volutpat. Etiam lacus ante, euismod in lacinia eget, mollis a erat. Vestibulum at rhoncus odio. Morbi sed ligula in lacus placerat mollis. Nullam nec odio vel elit ultricies varius et et dolor. Integer id ante lacus. Duis dictum, massa sed bibendum consequat, nibh enim iaculis nunc, vitae volutpat lorem tortor ut lorem.\n"
"\n"
"Proin vel mi lorem, vitae aliquet eros. Integer dapibus est gravida eros adipiscing pulvinar. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Pellentesque faucibus posuere lorem at porttitor. Phasellus consectetur porta ligula et fringilla. Aliquam arcu quam, posuere sit amet bibendum sit amet, consequat et risus. Cras blandit egestas libero. Aenean consequat placerat felis, faucibus tincidunt erat tristique et. Mauris mauris ligula, tempor vel porta sit amet, dictum sit amet eros. Ut magna velit, vulputate a vehicula non, consectetur sit amet libero.\n"
"\n"
"Vestibulum auctor lectus ac odio auctor in ultrices leo consectetur. Aliquam elementum laoreet libero eget tempus. Aenean eget ullamcorper libero. Aenean vulputate libero eros, non commodo sem. Aliquam sit amet nibh mi. Nam hendrerit tincidunt scelerisque. Quisque condimentum bibendum malesuada. Maecenas nibh lorem, commodo quis tempus rhoncus, accumsan eget eros. Aliquam suscipit, est eu venenatis rhoncus, elit enim porta est, eget semper lorem metus id arcu. Fusce feugiat iaculis nulla eget sagittis. Aliquam feugiat feugiat metus sit amet ornare. Vivamus arcu lectus, lacinia consectetur molestie vel, malesuada vitae est. Proin vitae dui sem. Nulla congue augue sollicitudin dui pretium a volutpat ligula ultricies. Sed tortor purus, tincidunt vel consectetur ac, sollicitudin vitae odio. Nunc a commodo tortor.\n"
"\n"
"Nam mollis nibh sed ante pulvinar ultrices. Curabitur vel est lorem. Suspendisse potenti. Pellentesque in augue diam, eget luctus justo. Praesent et odio neque. Integer mattis suscipit porttitor. Fusce tempor gravida quam, sit amet ultricies nunc lacinia ac. Sed non mi sit amet tellus sollicitudin rhoncus. Nulla facilisi. Phasellus ut tellus a magna porta rhoncus. Vivamus consequat consequat nisi, quis tempus urna feugiat ac. Praesent eu dolor eros, in pulvinar enim. Etiam in sapien sed enim vestibulum bibendum sit amet at arcu. In hac habitasse platea dictumst. Ut accumsan, nisl a fringilla ultricies, lacus felis hendrerit tortor, nec tincidunt ligula neque in est. Nunc eget sed.\n";

	char sig[] = 
	{0x72,0x73,0x01,0x36,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x08,0x1F,0x45,0xE7
	,0xAB,0x32,0xB9,0x02,0x40,0x53,0xF6,0x62,0x6B,0x48,0x75,0xD3,0x07,0xB3,0xF1
	,0xF9,0x5B,0xE0,0xC7,0x3C,0x17};


	SimpleBufferStream outputStream;
	std::stringstream ss(longer);
	{
		ToSignatureStream stream(outputStream, 8);
		SimpleIStreamStream ssStream(ss);
		// 10 bytes will be written on every write. This simulates huge files. 
		StreamUtilities::copyStream(ssStream, stream, 10, 3007);
	}

	unsigned int n;
	char *actual = outputStream.getBuffer(&n);

	ASSERT_EQ(sizeof(sig), n);

	for(unsigned int i = 0; i < n; i++)
	{
		ASSERT_EQ(sig[i], actual[i]);
	}
}

TEST( SignatureStreamTest, DumbSignatureLoadTest )
{
	LoadSignatureStream lss;

	{
		ToSignatureStream stream(lss, 8);
		stream.write("hello world!", 12);
	}
}

TEST( SignatureStreamTest, CompleteSimpleTest )
{
	char newStr[] = "Hello world!";
	char oldStr[] = "Hello to the world?";

	SimpleBufferStream tempStream;

	{
		ToSignatureStream stream(tempStream, 8);
		stream.write(newStr, 12);
	}


	LoadSignatureStream lss;

	ToSignatureStream stream(lss, 8);
	StreamUtilities::copyStream(tempStream, stream); 

	SimpleBufferStream outputStream;
	PerformDeltaStream deltaStream(outputStream, lss.getSignature());
	
}

}
