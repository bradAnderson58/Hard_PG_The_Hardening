varying vec3 vNormal;
varying vec3 vVertex;

uniform float silhouetteThreshold;
uniform sampler2D tex;

void main (void)
{

   vec4 materialColor = gl_FrontMaterial.diffuse;

   vec4 silhouetteColor = vec4(0.0, 0.0, 0.0, 1.0);

   vec4 specularColor = gl_FrontMaterial.specular; 

   vec3 eyePos = normalize(-vVertex);
   vec3 lightPos = gl_LightSource[0].position.xyz;

   vec3 Normal = normalize(vNormal);
   vec3 EyeVert = normalize(eyePos - vVertex);
   vec3 LightVert = normalize(vVertex - lightPos);
   vec3 EyeLight = normalize(LightVert + EyeVert);
   vec4 texture = texture2D(tex,gl_TexCoord[0].st);

   // make a silhouette around character
   float sil = max(dot(Normal,EyeVert), 0.0);
   if( sil < 0.15 )
      gl_FragColor = silhouetteColor;
   // inside silhouette
   else
   {
      gl_FragColor = materialColor*texture;

      float spec = pow(max(dot(Normal,EyeLight), 0.0), 5.0);
      if( spec < 0.03 )
         gl_FragColor *= 0.9;
      else
         gl_FragColor = specularColor*texture;

      float diffuse = max(dot(Normal,LightVert), 0.0);
      if( diffuse < 0.3 )
         gl_FragColor *= 0.9;


   }
}