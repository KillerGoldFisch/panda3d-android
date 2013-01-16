### Generated automatically by ppremake 1.22 from panda.prc.pp.
################################ DO NOT EDIT ###########################
#plugin-path $THIS_PRC_DIR/../lib
# Let's set up a default window size of 800x600.  The user can
# override this in his or her personal prc file.
#win-size 800 600
# Define the display types that have been compiled in.  Panda will
# pick one of these by going through the list in this order until one
# is found that works, unless the user specifically requests a
# particular display type with the load-display directive.

#win-origin 0 0
#win-size 800 480 
fullscreen 1
#depth-bits 1
#show-frame-rate-meter 1

show-frame-rate-meter 1
load-display p3androidegldisplay
#aux-display pandagles2
#aux-display egldisplay
#aux-display pandagl
#aux-display p3tinydisplay
#aux-display p3tinydisplay



# Define an appropriate default audio library.
# The egg loader is handy to have available by default.  This allows
# clients to load egg files.  (The bam loader is built-in so bam files
# are always loadable).
# By qualifying with the extension "egg", we indicate the egg loader
# should be made available only if you explicitly name a file with an
# .egg extension.
# Also see ptloader, which is built as part of pandatool; it allows
# files of more exotic types (like .flt, .mb, .lwo, and .dxf) to be
# loaded directly into Panda.
load-file-type egg pandaegg
# The following lines define some handy object types to use within the
# egg syntax.  This remaps <ObjectType> { name } into whatever egg
# syntax is given by egg-object-type-name, which makes a handy
# abbreviation for modeling packages (like Maya) to insert
# sophisticated egg syntax into the generated egg file, using a single
# object type string.
egg-object-type-portal          <Scalar> portal { 1 }
egg-object-type-polylight       <Scalar> polylight { 1 }
egg-object-type-seq24           <Switch> { 1 } <Scalar> fps { 24 }
egg-object-type-seq12           <Switch> { 1 } <Scalar> fps { 12 }
egg-object-type-seq10           <Switch> { 1 } <Scalar> fps { 10 }
egg-object-type-seq8            <Switch> { 1 } <Scalar> fps { 8 }
egg-object-type-seq6            <Switch> { 1} <Scalar>  fps { 6 }
egg-object-type-seq4            <Switch> { 1} <Scalar>  fps { 4 }
egg-object-type-seq2            <Switch> { 1} <Scalar>  fps { 2 }
egg-object-type-indexed         <Scalar> indexed { 1 }
egg-object-type-binary          <Scalar> alpha { binary }
egg-object-type-dual            <Scalar> alpha { dual }
egg-object-type-glass           <Scalar> alpha { blend_no_occlude }
# These are just shortcuts to define the Model and DCS flags, which
# indicate nodes that should not be flattened out of the hierarchy
# during the conversion process.  DCS goes one step further and
# indicates that the node's transform is important and should be
# preserved (DCS stands for Dynamic Coordinate System).  Notouch is
# even stronger, and means not to do any flattening below the node at
# all.
egg-object-type-model           <Model> { 1 }
egg-object-type-dcs             <DCS> { 1 }
egg-object-type-notouch         <DCS> { no_touch }
# The following define various kinds of collision geometry.  These
# mark the geometry at this level and below as invisible collision
# polygons, which can be used by Panda's collision system to detect
# collisions more optimally than regular visible polygons.
egg-object-type-barrier         <Collide> { Polyset descend }
egg-object-type-sphere          <Collide> { Sphere descend }
egg-object-type-invsphere       <Collide> { InvSphere descend }
egg-object-type-tube            <Collide> { Tube descend }
# As above, but these are flagged to be "intangible", so that they
# will trigger an event but not stop an object from passing through.
egg-object-type-trigger         <Collide> { Polyset descend intangible }
egg-object-type-trigger-sphere  <Collide> { Sphere descend intangible }
# "floor" and "dupefloor" define the nodes in question as floor
# polygons.  dupefloor means to duplicate the geometry first so that
# the same polygons serve both as visible geometry and as collision
# polygons.
egg-object-type-floor           <Collide> { Polyset descend level }
egg-object-type-dupefloor       <Collide> { Polyset keep descend level }
# "bubble" puts an invisible bubble around an object, but does not
# otherwise remove the geometry.
egg-object-type-bubble          <Collide> { Sphere keep descend }
# "ghost" turns off the normal collide bit that is set on visible
# geometry by default, so that if you are using visible geometry for
# collisions, this particular geometry will not be part of those
# collisions--it is ghostlike.
egg-object-type-ghost           <Scalar> collide-mask { 0 }
# "glow" is useful for halo effects and things of that ilk.  It
# renders the object in add mode instead of the normal opaque mode.
egg-object-type-glow            <Scalar> blend { add }
# This isn't used in the egg loader, it controls a setting only within
# maya2egg itself.  So if it appears in an egg file, it means nothing.
egg-object-type-keep-all-uvsets
