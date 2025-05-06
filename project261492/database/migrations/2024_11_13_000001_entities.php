<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Support\Facades\Schema;

return new class extends Migration
{
    /**
     * Run the migrations.
     */
    public function up(): void
    {
        Schema::create('entity_types', function (Blueprint $table) {
            $table->id('id')->primary();
            $table->string('type', 32)->unique();
        });

        Schema::create('entities', function (Blueprint $table) {
            $table->id()->primary();
            $table->foreignId('type')->references('id')->on('entity_types');
            $table->string('description', 32)->nullable();
            $table->timestamp('created_at')->useCurrent();
        });
    }

    /**
     * Reverse the migrations.
     */
    public function down(): void
    {
        Schema::dropIfExists('entities');
        Schema::dropIfExists('entity_types');
    }
};
