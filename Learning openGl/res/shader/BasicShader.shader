#shader vertex
#version 330 core
layout(location = 0) in vec4 position;
layout(location = 1) in vec4 aColor; 
layout(location = 2) in vec2 aTexCoord;
uniform mat4 u_MVP;

out vec4 vColor;
out vec2 vTexCoord;

void main()
{
    gl_Position = u_MVP * position;
    vColor = aColor; 
    vTexCoord = aTexCoord;
}

#shader fragment
#version 330 core
in vec4 vColor; 
in vec2 vTexCoord;

uniform vec4 uColor = vec4(1,1,1,1);
uniform sampler2D uTexture;
uniform float levels = 4.0f; // Số bậc độ sáng bạn muốn chia (ví dụ: 3, 4, 5...)

layout(location = 0) out vec4 color;

void main()
{
    vec4 texColor = texture(uTexture, vTexCoord);
    
    // 1. Tính độ sáng (Luminance) của pixel theo chuẩn mắt người
    float brightness = dot(texColor.rgb, vec3(0.299f, 0.587f, 0.114f));
    
    // 2. Chia độ sáng thành các bậc (level) cố định
    float quantizedBrightness = floor(brightness * levels) / levels;
    
    // 3. Giữ nguyên màu gốc (hue/saturation) nhưng ép độ sáng về bậc đã chia
    // Tránh chia cho 0 bằng cách kiểm tra điều kiện an toàn
    vec3 finalRGB = (brightness > 0.001f) ? texColor.rgb * (quantizedBrightness / brightness) : vec3(0.0f);

    color = vec4(finalRGB, texColor.a) *uColor ;
}