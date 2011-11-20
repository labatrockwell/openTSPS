/** @namespace LAB.three */
LAB.three = LAB.three || {};

/** 
 @constructor 
 @extends THREE.MeshShaderMaterial
 */

LAB.three.Shader = function ( parameters ) {
   
   $.ajax({ async: false, type: 'GET', url: parameters.name.concat( ".vert"),
          success: function(data) { parameters.vertexShader = data.slice( 0, data.length ); } });
   
   $.ajax({ async: false, type: 'GET', url: parameters.name.concat( ".frag"),
          success: function(data) { parameters.fragmentShader = data.slice( 0, data.length ); }});
   
   THREE.MeshShaderMaterial.call( this, parameters );
   
};

LAB.three.Shader.prototype = new THREE.MeshShaderMaterial();
LAB.three.Shader.prototype.constructor = LAB.three.Shader;
LAB.three.Shader.prototype.supr = THREE.MeshShaderMaterial.prototype;


/**
 @function
 @public
 */