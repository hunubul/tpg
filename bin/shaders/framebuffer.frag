#version 430 core
in vec2 Texcoord;

out vec4 outColor;

layout(rgba8,binding=0) readonly uniform image2D frameBuffer;
layout(rgba8,binding=1) readonly uniform image2D ASCII;
layout(r16f ,binding=2) readonly uniform image2D ASCIIMaxIndexes;
layout(rgba8,binding=3) uniform image2D prevframeTexture;

layout(location = 0) uniform int ASCIION;
layout(location = $) uniform int dirtyX[$];
layout(location = $) uniform int dirtyY[$];
layout(location = $) uniform int dirtyWidth[$];
layout(location = $) uniform int dirtyHeight[$];

int screenWidth=$;
int screenHeight=$;
int FontX=$;
int FontY=$;
int FontSize=$;


bool ASCIIScreenDraw() {

	bool Draw = false;

	if(ASCIION==1) {
		vec2 xyClamped = vec2(
			Texcoord.x*screenWidth  - mod(Texcoord.x*screenWidth ,FontX),
			Texcoord.y*screenHeight - mod(Texcoord.y*screenHeight,FontY)
		);

		if( xyClamped.x < FontX )
			xyClamped.x = FontX;
		if( xyClamped.y < FontY )
			xyClamped.y = FontY;

		ivec2 coord   = ivec2( int(Texcoord.x*screenWidth),       int(Texcoord.y*screenHeight) );
		ivec2 loadPos = ivec2( mod(Texcoord.x*screenWidth,FontX), mod(Texcoord.y*screenHeight,FontY) );

		if(imageLoad(ASCII,
						ivec2(loadPos.x,
							imageLoad(ASCIIMaxIndexes,ivec2(xyClamped.x/FontX,xyClamped.y/FontY)).x*FontY + loadPos.y
						)
					).x==1.0 )
		{
			Draw = true;
		}

	}
	else if(ASCIION==0) {
		Draw = true;
	}

	return Draw;
}

void main()
{
	ivec2 fragCoord = ivec2(Texcoord.x*screenWidth, Texcoord.y*screenHeight);

	bool dirty = false;
	if(ASCIION==0) {
		dirty = true;
	} else {
		for(int i=0; i<$; i++) {
			if(dirtyX[i]*FontX <= fragCoord.x && fragCoord.x <= dirtyX[i]*FontX + dirtyWidth[i]*FontX && dirtyY[i]*FontY <= fragCoord.y && fragCoord.y <= dirtyY[i]*FontY + dirtyHeight[i]*FontY)
				dirty = true;
		}
	}

	if(dirty) {
		if( ASCIIScreenDraw() ) {
			outColor = imageLoad(frameBuffer, fragCoord );
			imageStore(prevframeTexture, fragCoord, imageLoad(frameBuffer, fragCoord ) );
		} else {
			outColor = vec4(0.0,0.0,0.0,0.0);
			imageStore(prevframeTexture, fragCoord, vec4(0.0,0.0,0.0,0.0) );
		}
	} else {
		outColor = imageLoad(prevframeTexture, fragCoord );
	}
}